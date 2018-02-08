# Sample Web application prototype in ReasonML

## Install Libraries

```
$ npm install
```

## Build

```
$ make build
```

## Run Server

```
$ make start
```

You can access "http://localhost:3000".

## Directory Structure

```
src/
    fulful/     # Flux implementation
    kanae/      # Base library
    models/     # Database models (Sequelaize)
    nozomi/     # Express binding
    public/     # Static files
    routes/     # Routers (Express)
    siika/      # Sequelize binding
    views/      # Templates (Pug)
```
