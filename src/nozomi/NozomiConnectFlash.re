module Basic = {
  [@bs.module]
  external flash : unit => NozomiMiddleware.Basic.f = "connect-flash";
};

let middleware = () => Basic.flash() |> NozomiMiddleware.from;