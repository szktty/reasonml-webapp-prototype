module Basic = {
  type t;
  [@bs.module "express"] external create : unit => t = "Router";
  [@bs.send]
  external use : (t, string, array(NozomiMiddleware.Basic.f)) => unit = "use";
  [@bs.send]
  external all : (t, string, array(NozomiMiddleware.Basic.f)) => unit = "all";
  [@bs.send]
  external get : (t, string, array(NozomiMiddleware.Basic.f)) => unit = "get";
  [@bs.send]
  external post : (t, string, array(NozomiMiddleware.Basic.f)) => unit =
    "post";
};

include
  NozomiRoutable.Make(
    {
      type router = Basic.t;
      let all = (router: router, ~path, ~handlers: list(NozomiMiddleware.f)) =>
        Basic.all(router, path, NozomiMiddleware.basicArray(handlers));
      let get = (router: router, ~path, ~handlers: list(NozomiMiddleware.f)) =>
        Basic.get(router, path, NozomiMiddleware.basicArray(handlers));
      let post = (router: router, ~path, ~handlers: list(NozomiMiddleware.f)) =>
        Basic.post(router, path, NozomiMiddleware.basicArray(handlers));
    }
  );

let create = Basic.create;

let use = (router, ~path, ~f) => Basic.use(router, path, f);
/*
 let middleware0 =
     (router, ~request: Request.t, ~response: Response.t, ~next: unit => unit)
     : handler =>
   (~request, ~response, ~next) => ();
 let middleware: t => handler = router => middleware0(router);
 */