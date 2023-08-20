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
    const body = req.body as SearchParameters;
    const response = JSON.stringify(
        cities.split('\n')
            .map(city => new GeonamesEntry(city, admin1Manager))
            .filter(city => city.hitsSearch(body.query))
            .sort((a, b) => b.population - a.population)
            .map(city => city.asObject())
    )

    res.header("Content-Type", "application/json");
    res.send(response);
});
Locations.post("/reverse", (req, res) => {
    const body = req.body as ReverseParameters;
    const sortedCities = cities.split('\n')
        .map(city => new GeonamesEntry(city, admin1Manager))
        .sort((a, b) => a.distanceTo(body.lat, body.lng) - b.distanceTo(body.lat, body.lng))
        .map(x => x.asObject())
    const response = JSON.stringify(sortedCities[0])

    res.header("Content-Type", "application/json");
    res.send(response);
})

interface SearchParameters {
    query: string;
}

interface ReverseParameters {
    lat: number,
    lng: number
}

export { Locations }