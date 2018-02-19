open Kanae.Base;

module Basic = {
  type t;
  type renderer = (unit /* error */, string) => [@bs.uncurry] unit;
  [@bs.module] external express : unit => t = "express";
  [@bs.send] external render : (t, Js.t('local), renderer) => unit = "render";
  [@bs.send] external listenPath : (t, string) => unit = "listen";
  [@bs.send]
  external listenPort : (t, int, Js.Null.t(string), Js.Null.t(string)) => unit =
    "listen";
  [@bs.send] external set : (t, string, 'value) => unit = "set";
  [@bs.send]
  external use : (t, string, NozomiMiddleware.Basic.f) => unit = "use";
  [@bs.send]
  external all : (t, string, NozomiMiddleware.Basic.f) => unit = "get";
  [@bs.send]
  external get : (t, string, NozomiMiddleware.Basic.f) => unit = "get";
  [@bs.send]
  external post : (t, string, NozomiMiddleware.Basic.f) => unit = "post";
  [@bs.send]
  external put : (t, string, NozomiMiddleware.Basic.f) => unit = "put";
  [@bs.send]
  external delete : (t, string, NozomiMiddleware.Basic.f) => unit = "delete";
};

module ViewEngine = {
  type t =
    | Jade
    | Pug;
  let toJS = engine =>
    switch engine {
    | Jade => "jade"
    | Pug => "pug"
    };
};

module Settings = {
  type t =
    | Views(list(string))
    | ViewEngine(ViewEngine.t);
};

type t = Basic.t;

type neverReturns = unit;

let create = Basic.express;

let set = (app, setting) =>
  switch setting {
  | Settings.Views(views) => Basic.set(app, "views", Js.fromList(views))
  | ViewEngine(engine) =>
    Basic.set(app, "view engine", ViewEngine.toJS(engine))
  };

let listen = (app, ~path) : neverReturns => Basic.listenPath(app, path);

let start = (~host=?, ~backlog=?, app, ~port: int) =>
  Basic.listenPort(
    app,
    port,
    Js.Null.fromOption(host),
    Js.Null.fromOption(backlog)
  );

let use =
    (
      ~path="*",
      app,
      ~f:
         (
           ~request: NozomiRequest.t,
           ~response: NozomiResponse.t,
           ~next: unit => unit
         ) =>
         unit
    ) =>
  Basic.use(app, path, (req, res, next) =>
    f(
      ~request=NozomiRequest.from(req),
      ~response=NozomiResponse.from(res),
      ~next
    )
  );

include
  NozomiRoutable.Make(
    {
      type router = t;
      let all = (router: router, path, handler: NozomiMiddleware.f) =>
        Basic.all(router, path, NozomiMiddleware.basic(handler));
      let get = (router: router, path, handler: NozomiMiddleware.f) =>
        Basic.get(router, path, NozomiMiddleware.basic(handler));
      let post = (router: router, path, handler: NozomiMiddleware.f) =>
        Basic.post(router, path, NozomiMiddleware.basic(handler));
    }
  );