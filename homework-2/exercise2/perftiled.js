const exec = require('child_process').exec;
const fs = require('fs')
const command1 = 'perf stat -o perftiled.csv --append -x , -e L1-dcache-load-misses,LLC-load-misses ./tiled 30000';
const command2 = 'perf stat -o perfarray_init.csv --append -x , -e L1-dcache-load-misses,LLC-load-misses ./array_init 30000';

async function sh(cmd) {
  return new Promise(function (resolve, reject) {
    exec(cmd, (err, stdout, stderr) => {
      if (err) {
        reject(err);
      } else {
        resolve({ stdout, stderr });
      }
    });
  });
}

async function main() {
	fs.open('perftiled.csv', 'w', () => {} );
	fs.open('perfarray_init.csv', 'w', () => {});
	let timeNontiled = 0;
  	for(let i=0; i<5; i++) {
		await sh(command2);
  	}

	const trails = [1, 16, 64, 256, 1024];
	for(const num of trails) {
		let timeTiled = 0;
	        for(let i=0; i<5; i++) {
        	        const out = await sh(command1 + ' ' + num);
			timeTiled += parseFloat(out.stdout);
	        }
		console.log('Elasped Time is ' + timeTiled/5 + ' @ B= ' + num);  
	}

        let ini_dcacheMiss = 0;
        let ini_llcMiss = 0;
        const ini_result = fs.readFileSync('./perfarray_init.csv', {encoding: 'utf-8'});
        const ini_lines = ini_result.split('\n');
        for(const line of ini_lines) {
                const columns = line.split(',');
                if(columns[2] === 'L1-dcache-load-misses:u') ini_dcacheMiss += parseInt(columns[0]);
                if(columns[2] === 'LLC-load-misses:u') ini_llcMiss += parseInt(columns[0]);
        }
	console.log('total ini L1 miss: ' + ini_dcacheMiss);
        console.log('total ini LLC miss: ' + ini_llcMiss);
        console.log('avg of initial L1, LLC are: ' + ini_dcacheMiss/5 + '|' + ini_llcMiss/5);
	
	let counter = 0;
	let index = 0;
	let dcacheMiss = 0;
	let llcMiss = 0;
  	const csv = fs.readFileSync('./perftiled.csv', {encoding: 'utf-8'});
  	const lines = csv.split('\n');
  	for(const line of lines) {
		const columns = line.split(',');
		if(columns[2] === 'L1-dcache-load-misses:u') dcacheMiss += parseInt(columns[0]);
		if(columns[2] === 'LLC-load-misses:u') {
			llcMiss += parseInt(columns[0]);
			counter++;
		}
		if(counter === 5) {
			counter = 0;
			console.log('B='+ trails[index]);
			console.log('L1 Miss:  ' + (dcacheMiss/5 - ini_dcacheMiss/5));
			console.log('LLC Miss: ' + (llcMiss/5 - ini_llcMiss/5));
			index++;
			dcacheMiss = 0;
			llcMiss = 0;			
		}
  	}
}

main();