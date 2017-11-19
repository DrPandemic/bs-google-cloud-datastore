open GoogleCloudDatastore;

let app = Datastore.make ();

let key0 = Datastore.keyByKind app "test";
let key1 = Datastore.keyByID app ("test", 42);
let key2 = Datastore.keyByName app ("test", "name");
let key3 = Datastore.keyByPathAndID app {"namespace": "wow", "path": ("test", 42)};
let key4 = Datastore.keyByPathAndName app {"namespace": "wow", "path": ("test12", "wow")};

let testSave () => {
  Datastore.save app {"key": key1, "data": { "name": "that name", "yes": "42" }} (fun _ _ => {
    Datastore.save app {"key": key2, "data": { "name": "second", "yes": 42 }} (fun _ _ => {
      Datastore.getMultiple app [| key1, key2 |] (fun a b => {
        Js.log a;
        Js.log b;
      });
    });
  });
};

let testDelete () => {
  Datastore.delete app key0 (fun a b => {
    Js.log a;
    Js.log b;
  });
};

let testQuery () => {
  let query = Datastore.createQuery app "test"
                |> Query.order "name"
                |> Query.filterWithOperator "name" ">" (Js.Json.string "f")
                |> Query.limit 5
                |> Query.select "name";

  /* let q1 = Query.hasAncestor key1 query; */

  Query.runWithConsistency query Strong (fun error results info => {
    Js.log error;
    Js.log results;
    Js.log info;
  });
};

let testGeo () => {
  let geo = Datastore.geoPoint app {"latitude": 1.1, "longitude": 2.2};
  Js.log geo;
};

let testAllocateIds () => {
  Datastore.allocateIds app key0 10 (fun error results info => {
    Js.log results;
  });
};

let testPromise () => {
  let query = Datastore.createQuery app "test"
                |> Query.order "name"
                |> Query.limit 2;

  Query.runPromise query
    |> Js.Promise.then_ (fun results => {
      Js.log results;
      Js.Promise.resolve ()
    }
  );
};

/* testDelete (); */
/* testSave (); */
testQuery ();
/* testAllocateIds (); */
/* testGeo (); */
/* testPromise (); */
