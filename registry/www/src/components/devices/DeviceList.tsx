import React, {useEffect, useMemo, useRef, useState} from "react";
import DeviceRepository from "../../repository/DeviceRepository";
import RegistryRepository from "../../repository/RegistryRepository";
import {useTable} from "react-table";
import Pagination from "@material-ui/lab/Pagination";


const DeviceList = () => {
    const [devices, setDevices] = useState([]);
    const [registryId, setRegistryId] = useState(0);
    const [registries, setRegistries] = useState([]);
    const [searchName, setSearchName] = useState("");
    const devicesRef = useRef([]);

    const [page, setPage] = useState(1);
    const [count, setCount] = useState(0);
    const [pageSize, setPageSize] = useState(20);

    devicesRef.current = devices;

    const onChangeSearchName = (e: any) => {
        const searchName = e.target.value;
        setSearchName(searchName);
    };

    const getRequestParams = (searchName: string, registryId : number, page: number, pageSize: number) => {
        let params: any = {};

        if (searchName) {
            params["name"] = searchName;
        }

        if (registryId) {
            params["registry_id"] = registryId;
        }

        if (page) {
            params["page"] = page - 1;
        }

        if (pageSize) {
            params["size"] = pageSize;
        }

        return params;
    };

    const findAllRegistries = () => {
        RegistryRepository.findAll({})
            .then((response) => {
                setRegistries(response.data.data);
            })
            .catch((e) => {
                console.log(e);
            });
    };

    const findAllDevices = () => {
        const params = getRequestParams(searchName, registryId, page, pageSize);

        DeviceRepository.findAll(params)
            .then((response) => {
                setDevices(response.data.data);
                setCount(Math.ceil(response.data.total / pageSize));
            })
            .catch((e) => {
                console.log(e);
            });
    };

    const findByName = () => {
        setPage(1);
        findAllDevices();
    }

    useEffect(() => {
        findAllDevices()
        findAllRegistries()
    }, [searchName, registryId, page, pageSize]);

    const openDevice = (rowIndex: any) => {
        // @ts-ignore
        const id = registriesRef.current[rowIndex].id;

        //props.history.push("/registry/" + id);
    };

    const deleteDevice = (rowIndex: any) => {
        // @ts-ignore
        const id = registriesRef.current[rowIndex].id;

        DeviceRepository.remove(id)
            .then((response) => {
                //props.history.push("/registry");

                let newRegistries = [...devicesRef.current];
                newRegistries.splice(rowIndex, 1);

                setDevices(newRegistries);
            })
            .catch((e) => {
                console.log(e);
            });
    };

    const handlePageChange = (event: any, value: number) => {
        setPage(value);
    };

    const handleRegistryChange = (e: any) => {
        setRegistryId(parseInt(e.target.value, 10))
        findAllDevices()
    }


    const columns = useMemo(
        () => [
            {
                Header: "#",
                accessor: "id",
            },
            {
                Header: "Name",
                accessor: "name",
            },
            {
                Header: "UUID",
                accessor: "uuid",
            },
            {
                Header: "Status",
                accessor: "json_data",
                Cell: (props: any) => {
                    if (props.value) {
                        return props.value.status;
                    } else {
                        return "unknown"
                    }
                },
            },
            {
                Header: "Actions",
                accessor: "actions",
                Cell: (props) => {
                    const rowIdx = props.row.id;
                    return (
                        <div>
                            <span onClick={() => openDevice(rowIdx)}>
                            <i className="far fa-edit action mr-2"></i>
                            </span>

                            <span onClick={() => deleteDevice(rowIdx)}>
                            <i className="fas fa-trash action"></i>
                            </span>
                        </div>
                    );
                },
            },
        ],
        []
    );

    // @ts-ignore
    const {
        getTableProps,
        getTableBodyProps,
        headerGroups,
        rows,
        prepareRow,
    } = useTable({
        // @ts-ignore
        columns,
        data: devices,
    });

    return (
        <div className="card">
            <div className="row list">
                <div className="col-md-12 list">
                    <div className="card-header">
                        <h3 className="card-title">Devices</h3>
                    </div>
                    <div className="card-body row">
                        <div className="col-md-6">
                            <div className="input-group mb-3">
                                <input
                                    type="text"
                                    className="form-control"
                                    placeholder="Search by name"
                                    value={searchName}
                                    onChange={onChangeSearchName}
                                />
                                <div className="input-group-append">
                                    <button
                                        className="btn btn-outline-secondary"
                                        type="button"
                                        onClick={findByName}
                                    >
                                        Search
                                    </button>
                                </div>
                            </div>
                        </div>
                        <div className="input-group mb-3 col-md-6">
                            <div className="input-group-prepend">
                                <label htmlFor="registries" className="input-group-text">Registry</label>
                            </div>
                            <select id="registries" className="custom-select" onChange={handleRegistryChange}>
                                <option selected value={0}>Choose...</option>
                                {registries.map((registry) => (
                                    // @ts-ignore
                                    <option key={registry.name} id={registry.id}
                                        // @ts-ignore
                                            value={registry.id}>{registry.name}</option>
                                ))}
                            </select>
                        </div>
                        <table
                            className="table table-striped table-bordered"
                            {...getTableProps()}
                        >
                            <thead>
                            {headerGroups.map((headerGroup, headerId) => (
                                <tr {...headerGroup.getHeaderGroupProps()} key={headerId}>
                                    {headerGroup.headers.map((column) => (
                                        <th {...column.getHeaderProps()} key={column.id}>
                                            {column.render("Header")}
                                        </th>
                                    ))}
                                </tr>
                            ))}
                            </thead>
                            <tbody {...getTableBodyProps()}>
                            {rows.map((row, i) => {
                                prepareRow(row);
                                return (
                                    <tr {...row.getRowProps()} key={row.id}>
                                        {row.cells.map((cell, id) => {
                                            return (
                                                <td {...cell.getCellProps()} key={id}>{cell.render("Cell")}</td>
                                            );
                                        })}
                                    </tr>
                                );
                            })}
                            </tbody>
                        </table>
                        <div className="col-md-12">
                            <div className="card-tools">
                                <Pagination
                                    className="pagination pagination-sm float-right"
                                    count={count}
                                    page={page}
                                    siblingCount={1}
                                    boundaryCount={1}
                                    variant="outlined"
                                    shape="rounded"
                                    size={"small"}
                                    onChange={handlePageChange}
                                />
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    );
};

export default DeviceList;