open! Kanae;

open Nozomi;

/* create Express */
let app = Nozomi.express();

/* set template engine */
App.set(app, App.Settings.ViewEngine(Pug));

/* set template files */
App.set(app, App.Settings.Views([Path.join(Path.cwd(), "views")]));

App.use(app, ~handlers=[Session.middleware(~secret="foobarbaz", ())]);

App.use(app, ~handlers=[ConnectFlash.middleware()]);

App.use(app, ~handlers=[Passport.middleware()]);

Passport.use(
  Passport.LocalStrategy.create(~f=(~username, ~password) => {
    Js.log("--- LocalStrategy");
    Js.log(username);
    Js.log(password);
    Passport.Done.OK("ok");
  })
);

App.use(
  app,
  ~path="/login",
  ~handlers=[
    Passport.Authenticator.local(
      ~successRedirect="/",
      ~failureRedirect="/",
      ~failureFlash=true
    )
  ]
);

/* body parser */
/*App.use(app, ~f=BodyParser.json());*/
App.use(app, ~handlers=[BodyParser.urlencoded()]);

App.use(app, ~handlers=[CookieParser.create()]);

/* get "/" */
App.use(app, ~path="/", ~handlers=[Index.handler]);

App.use(app, ~path="/login", ~handlers=[Login.login]);