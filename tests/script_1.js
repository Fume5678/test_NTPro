import http, { get } from 'k6/http';
import { check, sleep } from 'k6';


const params = {
    headers: {
        'Content-Type': 'application/json',
    },
};

function user1() {
    let res = http.post('http://localhost:1234/api/add_order', JSON.stringify({
        user_id: "123123120",
        source: "USD",
        target: "RUB",
        type: "BUY",
        value: 10,
        price: 62
    }),
        params
    );

    check(res, {
        'status is 200': (r) => r.status === 200
    });
    sleep(0.3);
}

function user2() {
    let res = http.post('http://localhost:1234/api/add_order', JSON.stringify({
        user_id: "123123121",
        source: "USD",
        target: "RUB",
        type: "BUY",
        value: 20,
        price: 63
    }),
        params
    );

    check(res, {
        'status is 200': (r) => r.status === 200
    });
    sleep(0.3);
}

function user3() {
    let res = http.post('http://localhost:1234/api/add_order', JSON.stringify({
        user_id: "123123122",
        source: "USD",
        target: "RUB",
        type: "SELL",
        value: 50,
        price: 61
    }),
        params
    );

    check(res, {
        'status is 200': (r) => r.status === 200
    });
    sleep(0.3);
}

export default function () {

    for (var i = 0; i < 3; i++) {
        const payload = JSON.stringify({
            user_id: '12312312' + i,
        });



        let res = http.post('http://localhost:1234/api/add_user', payload, params);
        check(res, { 'success login': (r) => r.status === 200 });
        sleep(0.3);
    }

    user1();
    user2();
    user3();

    let userdetail_0 = JSON.parse(http.post('http://localhost:1234/api/get_userdetail', JSON.stringify({
        user_id: "123123120",
    }),
        params
    ).body);
    sleep(0.3);

    let userdetail_1 = JSON.parse(http.post('http://localhost:1234/api/get_userdetail', JSON.stringify({
        user_id: "123123121",
    }),
        params
    ).body);
    sleep(0.3);

    let userdetail_2 = JSON.parse(http.post('http://localhost:1234/api/get_userdetail', JSON.stringify({
        user_id: "123123122",
    }),
        params
    ).body);    
    sleep(0.3);

    console.log(userdetail_0["balance"]);
    console.log(userdetail_1["balance"]);
    console.log(userdetail_2["balance"]);

    check(userdetail_0, 
        {
            '0 USD balance: ': (r) => parseInt(r["balance"]["USD"]) == 10,
            '0 RUB balance: ': (r) => parseInt(r["balance"]["RUB"]) == -620,
        }
    );
    check(userdetail_1, 
        {
            '1 USD balance: ': (r) => r["balance"]["USD"] == 20,
            '1 RUB balance: ': (r) => r["balance"]["RUB"] == -1260,
        }
    );
    check(userdetail_2, 
        {
            '2 USD balance: ': (r) => r["balance"]["USD"] == -30,
            '2 RUB balance: ': (r) => r["balance"]["RUB"] == 1880,
        }
    );

    let user2_orders = JSON.parse(http.post('http://localhost:1234/api/get_user_orders', JSON.stringify({
        user_id: "123123122",
    }),
        params
    ).body);
    sleep(0.3);

    console.log(user2_orders);
    check(user2_orders, {
        'user2_orders size': (r) => r["orders"].length === 1,
        'user2_orders val': (r) => r["orders"][0]["value"] === 20,
    });



}
