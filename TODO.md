# TODO

-[ ] IotPlatform должна иметь два класса
-[ ] IotDevice
    - может подписаться на стандартные registryId топики 
      - sub /{registryId-id}/config
    - может подписаться на brodcast топики 
      - sub /{registryId-id}/broadcast
    - может посылать данные в стандартные device топики 
      - pub /{registryId-id}/{device-id}/telemetry 
      - pub /{registryId-id}/{device-id}/rpc
-[ ] IotRegistry
    - может подписаться на все cтандартные device топики 
      - sub /{registryId-id}/+/telemetry
      - sub /{registryId-id}/+/config
    - может посылать данные в broadcast топики 
      - pub /{registryId-id}/broadcast
    - может посылать данные в конкретные device топики 
      - pub /{registryId-id}/{device-id}/config
      - pub /{registryId-id}/{device-id}/rpc

## Iot IotRegistry

| topic                    | description   | registryId | device  |
|--------------------------|---------------|----------|---------|
| /{registryId-id}/broadcast | broadcast     | pub/sub  | pub/sub |
| /{registryId-id}/config    | global config | pub      | sub     |
| /{registryId-id}/rpc       | global rpc    | sub      | pub     |

## Iot Device

| topic                                | description      | device | registryId | 
|--------------------------------------|------------------|--------|----------|
| /{registryId-id}/{device-id}/telemetry | device telemetry | pub    | sub      |
| /{registryId-id}/{device-id}/config    | device config    | sub    | pub      | 
| /{registryId-id}/{device-id}/rpc       | device rpc       | sub    | pub      |



