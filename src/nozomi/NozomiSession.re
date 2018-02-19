open Kanae.Base;

module Basic = {
  type options = {
    .
    "cookie": {
      .
      "path": string,
      "httpOnly": Js.Boolean.t,
      "secure": Js.Boolean.t,
      "maxAge": Js.Null.t(int),
      "sameSite": Js.False.t(string)
    },
    "secret": string,
    "resave": Js.Boolean.t
  };
  [@bs.module]
  external session : options => NozomiMiddleware.Basic.f = "express-session";
};

let middleware =
    (
      ~cookiePath="/",
      ~cookieHTTPOnly=true,
      ~cookieSecure=false,
      ~cookieMaxAge=?,
      ~cookieDomain=?,
      ~cookieExpires=?,
      ~cookieSameSite: [ | `None | `Lax | `Strict]=`None,
      ~resave=true,
      ~secret,
      ()
    ) =>
  Basic.session({
    "cookie": {
      "path": cookiePath,
      "httpOnly": Js.fromBool(cookieHTTPOnly),
      "secure": Js.fromBool(cookieSecure),
      "maxAge": Js.Null.fromOption(cookieMaxAge),
      "sameSite":
        switch cookieSameSite {
        | `None => Js.False.none()
        | `Lax => Js.False.some("lax")
        | `Strict => Js.False.some("strict")
        }
    },
    "resave": Js.fromBool(resave),
    "secret": secret
  })
  |> NozomiMiddleware.from;