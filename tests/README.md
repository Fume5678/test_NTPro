## Google tests
```
    # В папке tests
    mkdir build
    cmake -S . -B build
    cmake --build build
    ./build/server_tests
```

## Запуск k6 скриптов 
Данные скрипты используются для тестирования АПИ сервера.

**Предварительно требуется запустить сервер.**
```
    k6 run script_1.js
    k6 run script_2.js
```

После запуска должны быть пройдены тесты.
