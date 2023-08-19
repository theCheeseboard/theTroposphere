class Admin1ManagerEntry {
	private idString: string;
	private nameString: string;

	constructor(line: string) {
		[
			this.idString,
			this.nameString
		] = line.split('\t');
	}

	get name() {
		return this.nameString;
	}

	get id() {
		return this.idString;
	}
}

export class Admin1Manager {
	private readonly entries: {[key: string]: Admin1ManagerEntry}

	constructor(file: string) {
		this.entries = {};

		for (let line of file.split('\n')) {
			const entry = new Admin1ManagerEntry(line);
			this.entries[entry.id] = entry;
		}
	}

	entry(country: string, division: string) {
		return this.entries[`${country}.${division}`];
	}
}
