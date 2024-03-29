--- cleanup
DROP TABLE IF EXISTS registry_configuration;
DROP TABLE IF EXISTS device_configuration;
DROP TABLE IF EXISTS device_telemetry;
DROP TABLE IF EXISTS device;
DROP TABLE IF EXISTS registryId;

CREATE TABLE IF NOT EXISTS registry
(
    id         SERIAL PRIMARY KEY,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    name       varchar(64) UNIQUE                                    NOT NULL,
    uuid       varchar(36) UNIQUE                                    NOT NULL,
    status     int                                                   NOT NULL DEFAULT 0,
    json_data  jsonb                                                 NULL
);

CREATE INDEX IF NOT EXISTS registry_idx_status_name ON registryId (status, name);

CREATE TABLE IF NOT EXISTS device
(
    id          SERIAL PRIMARY KEY,
    registry_id int                                                   NOT NULL REFERENCES registryId,
    created_at  timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at  timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    name        varchar(64) UNIQUE                                    NOT NULL,
    uuid        varchar(36) UNIQUE                                    NOT NULL,
    status      int                                                   NOT NULL DEFAULT 0,
    json_data   jsonb                                                 NULL
);

CREATE INDEX IF NOT EXISTS device_idx_status_name_registry ON device (status, name, registry_id);

CREATE TABLE IF NOT EXISTS registry_configuration
(
    id          SERIAL PRIMARY KEY,
    registry_id int                                                   NOT NULL REFERENCES registryId,
    created_at  timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at  timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    status      int                                                   NOT NULL DEFAULT 0,
    json_data   jsonb
);

CREATE TABLE IF NOT EXISTS device_telemetry
(
    id         SERIAL PRIMARY KEY,
    device_id  int                                                   NOT NULL REFERENCES device,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    json_data  jsonb                                                 NULL
);


CREATE TABLE IF NOT EXISTS device_configuration
(
    id         SERIAL PRIMARY KEY,
    device_id  int                                                   NOT NULL REFERENCES device (id),
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    updated_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    status     int                                                   NOT NULL DEFAULT 0,
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

INSERT INTO device (id, registry_id, name, uuid, json_data)
VALUES (2, 2, 'monitor', gen_random_uuid(), '{}')
ON CONFLICT DO NOTHING;

INSERT INTO device_configuration (id, device_id, status, json_data)
VALUES (1, 1, 1, '{
  "logging": {
    "level": "debug",
    "console": true,
    "file": false
  },
  "http": {
    "port": 8090
  }
}')
ON CONFLICT DO NOTHING;
