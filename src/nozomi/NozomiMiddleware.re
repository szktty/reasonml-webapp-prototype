open Kanae.Base;

module Basic = {
  type f =
    (
      NozomiRequest.Basic.t,
      NozomiResponse.Basic.t,
      unit => [@bs.uncurry] unit
    ) =>
    [@bs.uncurry] unit;
};

type f =
  (
    ~request: NozomiRequest.t,
    ~response: NozomiResponse.t,
    ~next: unit => unit
  ) =>
  [@bs.uncurry] unit;

let from = (f: Basic.f) : f =>
  (~request, ~response, ~next) =>
    f(NozomiRequest.from(request), NozomiResponse.from(response), next);

let bindBasic = (f, req, res, next) =>
  f(
    ~request=NozomiRequest.from(req),
    ~response=NozomiResponse.from(res),
    ~next
  );

let basic = (f: f) : Basic.f => bindBasic(f);

let basicArray = fs => List.map(fs, ~f=basic) |> List.toArray;