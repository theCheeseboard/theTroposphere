import { Admin1Manager } from './admin1-manager.js';

export class GeonamesEntry {
	private admin1Manager: Admin1Manager;

	private geonameId: string;
	private cityName: string;
	private asciiName: string;
	private alternateNamesString: string;
	private lat: string;
	private lng: string;
	private featureClass: string;
	private featureCode: string;
	private countryCode: string;
	private cc2: string;
	private admin1: string;
	private admin2: string;
	private admin3: string;
	private admin4: string;
	private populationString: string;
	private elevation: string;
	private dem: string;
	private timezone: string;

	constructor(line: string, admin1Manager: Admin1Manager) {
		this.admin1Manager = admin1Manager;

		[
			this.geonameId,
			this.cityName,
			this.asciiName,
			this.alternateNamesString,
			this.lat,
			this.lng,
			this.featureClass,
			this.featureCode,
			this.countryCode,
			this.cc2,
			this.admin1,
			this.admin2,
			this.admin3,
			this.admin4,
			this.populationString,
			this.elevation,
			this.dem,
			this.timezone
		] = line.split('\t');
	}

	asObject() {
		return {
			name: this.cityName,
			lat: parseFloat(this.lat),
			lng: parseFloat(this.lng),
			countryCode: this.countryCode,
			admin1Code: this.admin1,
			admin2Code: this.admin2,
			admin3Code: this.admin3,
			admin4Code: this.admin4,
			admin1: this.admin1Manager.entry(this.countryCode, this.admin1)?.name,
			population: this.population,
			timezone: this.timezone
		}
	}

	get alternateNames(): string[] {
		if (!this.alternateNamesString) return [];
		return this.alternateNamesString.split(',');
	}

	get population(): number {
		return parseInt(this.populationString);
	}

	hitsSearch(query: string) {
		if (this.asciiName?.toLowerCase()?.includes(query.toLowerCase())) return true;
		if (this.cityName?.toLowerCase()?.includes(query.toLowerCase())) return true;
		if (this.alternateNames?.some(name => name.toLowerCase().includes(query.toLowerCase()))) return true;
		return false;
	}

	distanceTo(lat: number, lng: number) {
		return Math.hypot(lat - parseFloat(this.lat), lng - parseFloat(this.lng));
	}
}
