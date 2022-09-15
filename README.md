# Test task NTPro
## Описание
Простая биржа с клиент-серверной архитектурой

## Зависимости
В разработке использовалась библиотека boost (https://www.boost.org/users/history/version_1_80_0.html), компонент asio. При сборке через cmake потребуется указать путь к корневой папке boost. 

Для тестирования запросов к серверу и использования скриптов из папки tests использовалась утилита k6 (https://k6.io/).

## Сборка проекта

```
    git clone git@github.com:Fume5678/test_NTPro.git
    cd test_NTPro
    
    mkdir build
    cd build 
    cmake ..  -D BOOST_ROOT=<path to bost root folder>
    make
```

Run server
```
    ./server/server
```

## Api сервера 

**[POST] /api/add_user** — Добавление пользователя

Content-Type: application/json

_Пример запроса_
```
{
    "user_id": "123123123"
}
```
---
**[POST] /api/add_order** — Добавление ордера 

Content-Type: application/json

_Пример запроса:_
```
{
    "user_id": "123123123",
    "source": "RUB",
    "target": "USD",
    "type": "BUY",
    "value": 20,
    "price": 61
}
```
---
**[POST] /api/get_user_orders** — Получение активных ордеров пользователя

Content-Type: application/json

_Пример запроса:_
```
{
    "user_id": "123123123"
}
```
---
**[POST] /api/get_userdetail** — Получение сведений о пользователе (в том числе его баланс

Content-Type: application/json

_Пример запроса:_
```
{
    "user_id": "123123123"
}
```