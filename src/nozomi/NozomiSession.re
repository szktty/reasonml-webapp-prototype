open Kanae;

module Basic = {
  type options = {
    .
    "cookie": {
      .
      "path": string,
      "httpOnly": Js.boolean,
      "secure": Js.boolean,
      "maxAge": Js.null(int),
      "sameSite": Js.False.t(string)
    },
    "secret": string,
    "resave": Js.boolean
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
      "maxAge": Js.Null.from_opt(cookieMaxAge),
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