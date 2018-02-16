module Basic = {
  [@bs.module]
  external flash : unit => NozomiMiddleware.Basic.f = "connect-flash";
};

let flash = () => Basic.flash() |> NozomiMiddleware.from;