import http, { get } from 'k6/http';
import { check, sleep } from 'k6';


function add_order() {
    const params = {
        headers: {
            'Content-Type': 'application/json',
        },
    };

    for (var i = 0; i < 15; i++) {
        let res = http.post('http://localhost:1234/api/add_order', JSON.stringify({
            user_id: parseInt(Math.random() * 2) == 1 ? "123123123" : "123123124",
            password: '123456',
            source: "USD",
            target: "RUB",
            type: parseInt(Math.random() * 2) == 1 ? "SELL" : "BUY",
            value: i*20,
            price: parseInt(Math.random() * 5) + 60,
        }),
            params
        );

        check(res, {
            'status is 200': (r) => r.status === 200
        });

        sleep(0.3);
    }
}

function get_orders() {
    const payload = JSON.stringify({
        source: "USD",
        target: "RUB",
        type: "SELL"
    });

    const params = {
        headers: {
            'Content-Type': 'application/json',
        },
    };

    let res = http.post('http://localhost:1234/api/get_orders', payload, params);
    check(res, { 'Orders SELL': (r) => r.status === 200 });
    console.log(JSON.parse(res.body));
    sleep(0.3);

    let payload2 = JSON.stringify({
        source: "USD",
        target: "RUB",
        type: "BUY"
    });


    let res2 = http.post('http://localhost:1234/api/get_orders', payload2, params);
    check(res2, { 'Orders BUY': (r) => r.status === 200 });

    console.log(JSON.parse(res2.body));

    sleep(0.3);
}


export default function () {

    const payload = JSON.stringify({
        user_id: '123123123',
        password: '123456'
    });

    const params = {
        headers: {
            'Content-Type': 'application/json',
        },
    };

    let res = http.post('http://localhost:1234/api/add_user', payload, params);
    check(res, { 'success login': (r) => r.status === 200 });
    sleep(0.3);

    const payload2 = JSON.stringify({
        user_id: '123123124',
        password: '123456'
    }); 

    let res2 = http.post('http://localhost:1234/api/add_user', payload2, params);
    check(res2, { 'success login': (r) => r.status === 200 });
    sleep(0.3);


    add_order();
    get_orders();
}
