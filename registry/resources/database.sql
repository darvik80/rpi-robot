--- cleanup
DROP TABLE IF EXISTS registry_configuration;
DROP TABLE IF EXISTS device_configuration;
DROP TABLE IF EXISTS device_telemetry;
DROP TABLE IF EXISTS device;
DROP TABLE IF EXISTS registryId;

CREATE TABLE IF NOT EXISTS registryId
(
    id         SERIAL PRIMARY KEY,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    name       varchar(64) UNIQUE NOT NULL,
    uuid       varchar(36) UNIQUE NOT NULL,
    status     int                NOT NULL DEFAULT 0,
    json_data  jsonb              NULL
    );

CREATE INDEX IF NOT EXISTS registry_idx_status_name ON registryId (status, name);

CREATE TABLE IF NOT EXISTS device
(
    id          SERIAL PRIMARY KEY,
    registry_id int                NOT NULL REFERENCES registryId,
    created_at  timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    name        varchar(64) UNIQUE NOT NULL,
    uuid        varchar(36) UNIQUE NOT NULL,
    status      int                NOT NULL DEFAULT 0,
    json_data   jsonb              NULL
    );

CREATE INDEX IF NOT EXISTS device_idx_status_name_registry ON device (status, name, registry_id);

CREATE TABLE IF NOT EXISTS device_telemetry
(
    id         SERIAL PRIMARY KEY,
    device_id  int                                                   NOT NULL REFERENCES device,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    json_data  jsonb                                                 NULL
);

CREATE TABLE IF NOT EXISTS registry_configuration
(
    id          SERIAL PRIMARY KEY,
    registry_id int                                                   NOT NULL REFERENCES registryId,
    created_at  timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    json_data   jsonb                                                 NULL
);


CREATE TABLE IF NOT EXISTS device_configuration
(
    id         SERIAL PRIMARY KEY,
    device_id  int                                                   NOT NULL REFERENCES device (id),
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    json_data  jsonb                                                 NULL
    );


INSERT INTO registryId (id, name, uuid, json_data)
VALUES (1, 'smart_home', '4d8a3176-c0c1-11ed-afa1-0242ac120002', '{
  "status": "online"
}')
    ON CONFLICT DO NOTHING;

INSERT INTO registryId (id, name, uuid, json_data)
VALUES (2, 'crearts-iot', 'c93af2d0-c0bf-11ed-afa1-0242ac120002', '{
  "status": "online"
}')
    ON CONFLICT DO NOTHING;

INSERT INTO device (id, registry_id, name, uuid, json_data)
VALUES (1, 2, 'rpi-robot', 'e114cd7c-c0c4-11ed-afa1-0242ac120002', '{
  "status": "online"
}')
    ON CONFLICT DO NOTHING;

SELECT name, json_data ->> 'status'
FROM registryId
WHERE json_data ->> 'status' = 'online';

SELECT d.name, r.name, d.json_data ->> 'status' status
FROM device d
    JOIN registryId r on r.id = d.registry_id