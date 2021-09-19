const exec = require('child_process').exec;
const fs = require('fs')
const command = 'perf stat -o perfnontiled.csv --append -x , -e L1-dcache-load-misses,LLC-load-misses ./nontiled 30000';
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
	fs.open('perfnontiled.csv', 'w', () => {} );
	fs.open('perfarray_init.csv', 'w', () => {});
	let timeNontiled = 0;
  	for(let i=0; i<5; i++) {
  		const time1 = await sh(command);
		timeNontiled += parseFloat(time1.stdout);
		await sh(command2);
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



	let dcacheMiss = 0;
	let llcMiss = 0;
  	const csv = fs.readFileSync('./perfnontiled.csv', {encoding: 'utf-8'});
  	const lines = csv.split('\n');
  	for(const line of lines) {
		const columns = line.split(',');
		if(columns[2] === 'L1-dcache-load-misses:u') dcacheMiss += parseInt(columns[0]);
		if(columns[2] === 'LLC-load-misses:u') llcMiss += parseInt(columns[0]);
  	}
	console.log('Elasped Time is ' + timeNontiled/5 + ' @B=30000');
	console.log('Array Initialize L1 Miss: ' + (ini_dcacheMiss/5));
	console.log('Array Initialize LLC Miss: ' + (ini_llcMiss/5));
	console.log('NonTiled L1 Miss: ' + (dcacheMiss/5 - ini_dcacheMiss/5));
	console.log('NonTiled LLC Miss: ' + (llcMiss/5 - ini_llcMiss/5));
}

main();