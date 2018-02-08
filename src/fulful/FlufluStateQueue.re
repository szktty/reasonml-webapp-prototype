module P = Js.Promise;

type update('state) = 'state => P.t('state);

type t('state) = {
  mutable state: 'state,
  middlewares: list(update('state)),
  mutable queue: list(update('state)),
  onUpdate: 'state => unit,
  onNext: (~prev: 'state, ~next: 'state) => unit,
  onComplete: 'state => unit
};

let default = (opt, default) =>
  switch opt {
  | None => default
  | Some(value) => value
  };

let create =
    (
      ~middlewares: list(update('state))=[],
      ~onUpdate=?,
      ~onNext=?,
      ~onComplete=?,
      ~state,
      ()
    ) => {
  state,
  middlewares,
  queue: [state => P.resolve(state)],
  onUpdate: default(onUpdate, _state => ()),
  onNext: default(onNext, (~prev as _, ~next as _) => ()),
  onComplete: default(onComplete, _state => ())
};

let apply =
    (middlewares: list('state => P.t('state)), state: 'state)
    : P.t('state) =>
  List.fold_left(
    (promise, middleware) => P.then_(state => middleware(state), promise),
    P.resolve(state),
    middlewares
  );

let add = (queue, update) => queue.queue = List.append(queue.queue, [update]);

let reduce = queue : P.t('state) => {
  let rec reduce0 = (promise: P.t('state)) : P.t('state) =>
    P.then_(
      prev => {
        let promise =
          P.then_(state => apply(queue.middlewares, state), promise);
        switch queue.queue {
        | [] => promise
        | [update, ...rest] =>
          queue.queue = rest;
          update(prev)
          |> P.then_(next => {
               queue.onNext(~prev, ~next);
               P.resolve(next);
             });
        };
      },
      promise
    )
    |> reduce0;
  reduce0(P.resolve(queue.state))
  |> P.then_(state => {
       queue.state = state;
       queue.onComplete(state);
       P.resolve(state);
     });
};

let update = (queue, ~f: update('state)) : P.t('state) => {
  queue.onUpdate(queue.state);
  add(queue, f);
  reduce(queue);
};