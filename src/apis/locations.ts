import express from "express"
import * as fs from "fs/promises";
import {GeonamesEntry} from "../helpers/geonames-entry.js";
import {Admin1Manager} from "../helpers/admin1-manager.js";

const cities = await fs.readFile("./dist/cities500.txt", {
    encoding: "utf-8"
});

const admin1Manager = new Admin1Manager(await fs.readFile("./dist/admin1CodesASCII.txt", {
    encoding: "utf-8"
}));

const Locations = express.Router();
Locations.post("/search", (req, res) => {
    const response = JSON.stringify(
        cities.split('\n')
            .filter(city => city.toLowerCase().includes(req.body.query.toLowerCase()))
            .map(city => new GeonamesEntry(city, admin1Manager))
            .sort((a, b) => b.population - a.population)
            .map(city => city.asObject())
    )

    res.header("Content-Type", "application/json");
    res.send(response);
});

export { Locations }