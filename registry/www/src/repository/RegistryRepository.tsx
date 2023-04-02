import axios from "../utils/axios";

const findAll = (params: any) => {
    return axios.get("/registries", {params});
};

const get = (id: bigint) => {
    return axios.get(`/registries/${id}`);
};

const create = (data: any) => {
    return axios.post("/registries", {
        params: data,
    });
};

const update = (id: bigint, data: any) => {
    return axios.put(`/registries/${id}`, data);
};

const remove = (id: bigint) => {
    return axios.delete(`/registries/${id}`);
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

export default RegistryRepository;