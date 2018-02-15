open Kanae;

module Basic = {
  [@bs.module]
  external m : array(string) => NozomiMiddleware.Basic.f = "cookie-parser";
};

let create = (~secret=[], ()) =>
  Basic.m(List.toArray(secret)) |> NozomiMiddleware.from;