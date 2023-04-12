import axios from 'axios';

export default axios.create({
    baseURL: "http://localhost:8080/api",
    headers: {
        "Content-type": "application/json"
    }
});

// const intance = axios.create({
//   baseURL: `${process.env.REACT_APP_GATEKEEPER_URL}`
// });
//
// intance.interceptors.request.use(
//   (request: AxiosRequestConfig<any>) => {
//     const {token} = store.getState().auth;
//     if (token) {
//       request.headers = {...request.headers, Authorization: `Bearer ${token}`};
//     }
//     return request;
//   },
//   (error) => {
//     return Promise.reject(error);
//   }
// );
//
// intance.interceptors.response.use(
//   (response) => response,
//   (error) => {
//     if (error.response.data.status === 401) {
//       store.dispatch(logoutUser());
//     }
//     return Promise.reject(error);
//   }
// );
//
// export default intance;
