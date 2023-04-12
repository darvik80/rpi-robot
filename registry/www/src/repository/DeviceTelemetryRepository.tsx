import axios from "../utils/axios";

const findAll = (params: any) => {
    return axios.get("/devices-telemetry", {params});
};

const get = (id: number) => {
    return axios.get(`/devices-telemetry?id=${id}`);
};

const remove = (id: number) => {
    return axios.delete(`/devices-telemetry?id=${id}`);
};

const removeAll = () => {
    return axios.delete(`/devices-telemetry`);
};

const DeviceTelemetryRepository = {
    findAll,
    get,
    remove,
    removeAll,
};

export default DeviceTelemetryRepository;