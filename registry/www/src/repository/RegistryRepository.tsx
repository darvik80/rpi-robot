import axios from "../utils/axios";

const findAll = (params: any) => {
    return axios.get("/registries", {params});
};

const get = (id: number) => {
    return axios.get(`/registries?id=${id}`);
};

const create = (data: any) => {
    return axios.post("/registries", {
        params: data,
    });
};

const update = (id: number, data: any) => {
    return axios.put(`/registries?id=${id}`, data);
};

const remove = (id: bigint) => {
    return axios.delete(`/registries?id=${id}`);
};

const removeAll = () => {
    return axios.delete(`/registries`);
};


const RegistryRepository = {
    findAll,
    get,
    create,
    update,
    remove,
    removeAll,
};

export default RegistryRepository