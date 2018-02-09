open! Kanae;

open Nozomi;

/* create Express */
let app = Nozomi.express();

/* set template engine */
App.set(app, App.Settings.ViewEngine(Pug));

/* set template files */
App.set(app, App.Settings.Views([Path.join(Path.cwd(), "views")]));

/* get "/" */
App.use(app, ~path="/", ~f=Index.handler);