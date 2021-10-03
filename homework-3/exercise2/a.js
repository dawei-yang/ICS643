let s = '{"id": "123", "value": "vyz"}\n\r{"id": "234", "value": "xiy"}';

try{
console.log(JSON.stringify(s));
s = '[' + s.replace(/}\n\r{/g, '},{') + ']';
console.log(JSON.stringify(s));
let obj = JSON.parse(s);
console.log(JSON.stringify(obj));
} catch(e){
    console.log(e);
}
