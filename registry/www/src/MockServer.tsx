// src/server.js
import {createServer, Model, RestSerializer} from "miragejs"

export function makeServer({environment = "test"} = {}) {
    return createServer({
        models: {
            registry: Model,
        },

        serializers: {
            registry: RestSerializer.extend({
                embed: true,
            }),
        },

        seeds(server) {
            server.create("registry", {
                // @ts-ignore
                id: 1,
                name: "smart_home",
                uuid: "4d8a3176-c0c1-11ed-afa1-0242ac120002",
                json_data: {status: "online"}
            })
            server.create("registry",
                {
                    // @ts-ignore
                    id: 2,
                    name: "crearts-iot",
                    uuid: "c93af2d0-c0bf-11ed-afa1-0242ac120002",
                    json_data: {status: "online"}
                }
            )
            server.create("registry",
                {
                    // @ts-ignore
                    id: 3,
                    name: "crearts-home",
                    uuid: "c93af2d0-c0bf-11ed-afa1-0242ac120004",
                    json_data: {status: "offline"}
                }
            )
            server.create("registry",
                {
                    // @ts-ignore
                    id: 4,
                    name: "alibaba",
                    uuid: "14066aa4-d134-11ed-afa1-0242ac120002",
                    json_data: {status: "offline"}
                }
            )
            server.create("registry",
                {
                    // @ts-ignore
                    id: 5,
                    name: "yandex",
                    uuid: "1f329e70-d134-11ed-afa1-0242ac120002",
                    json_data: {status: "online"}
                }
            )
        },

        routes() {
            this.get("/api/registries", (schema, request) => {
                let page = parseInt(request.queryParams.page)
                let size = parseInt(request.queryParams.size)
                let name = request.queryParams.name
                let offset = page * size
                let data: any = []

                // @ts-ignore
                schema.registries.all().models.forEach(item => {
                    if (name) {
                        if (item.name.startsWith(name)) {
                            data.push(item)
                        }
                    } else {
                        data.push(item)
                    }
                })

                return {
                    registries: data.slice(offset, offset + size),
                    total: data.length,
                }
            })
            let newId = 3
            this.post("/api/registries", (schema, request) => {
                let attrs = JSON.parse(request.requestBody)
                attrs.id = newId++

                // @ts-ignore
                return schema.registries.create(attrs)
            })
            this.delete("/api/registries/:id", (schema, request) => {
                // @ts-ignore
                return schema.registries.find(request.params.id).destroy()
            })
        },
    })
}
