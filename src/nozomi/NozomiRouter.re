module Basic = {
  type t;
  type handler =
    (
      NozomiRequest.Basic.t,
      NozomiResponse.Basic.t,
      unit => [@bs.uncurry] unit
    ) =>
    [@bs.uncurry] unit;
  [@bs.module "express"] external create : unit => t = "Router";
  [@bs.send] external use : (t, string, handler) => unit = "use";
};

type t = Basic.t;

type handler =
  (
    ~request: NozomiRequest.t,
    ~response: NozomiResponse.t,
    ~next: unit => [@bs.uncurry] unit
  ) =>
  unit;

let create = Basic.create;

let use = (router, ~path, ~f) => Basic.use(router, path, f);
/*
 let middleware0 =
     (router, ~request: Request.t, ~response: Response.t, ~next: unit => unit)
     : handler =>
   (~request, ~response, ~next) => ();
 let middleware: t => handler = router => middleware0(router);
 */