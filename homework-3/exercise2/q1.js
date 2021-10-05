const exec = require('child_process').exec;
const fs = require('fs')

const command = './exercise2 6000 100';


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
    
	let totalTime = 0;
	let max = 0;
	let min = Infinity;
	for(let i =0; i< 10; i++) {
 	    let time = await sh(command);
	    // console.log('time-> ' + JSON.stringify(time));
            time = parseFloat(time.stdout);
	    if(time > max) max = time;
	    if(time <= min) min = time;
	    totalTime += time; 
	}
	console.log(`Average elasped time is ${totalTime/10}`);
	console.log(`Min: ${min}  .   Max: ${max}`);
}


main();
