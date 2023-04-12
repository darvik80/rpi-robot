import axios from "../utils/axios";

const findAll = (params: any) => {
    return axios.get("/devices", {params});
};

const get = (id: number) => {
    return axios.get(`/devices?id=${id}`);
};

const create = (data: any) => {
    return axios.post("/devices", {
        params: data,
    });
};

const update = (id: number, data: any) => {
    return axios.put(`/devices?id=${id}`, data);
};

const remove = (id: number) => {
    return axios.delete(`/devices?id=${id}`);
};

const removeAll = () => {
    return axios.delete(`/devices`);
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