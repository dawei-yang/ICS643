const dim = 5;
for( let k = 0 ; k < dim * 2 ; k++ ) {
    console.log('parallel below: ');
    for( let j = 0 ; j <= k ; j++ ) {
        let i = k - j;
        if( i < dim && j < dim ) {
            console.log(`[${i}, ${j}]`);
        }
    }
    console.log('parallel done');
}