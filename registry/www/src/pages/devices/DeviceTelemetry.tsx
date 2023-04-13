import React, {useEffect, useMemo, useRef, useState} from "react";
import {DeviceRepository, DeviceTelemetryRepository} from '@repository';
import {useTable} from "react-table";
import Pagination from "@material-ui/lab/Pagination";
import {ContentHeader} from "@components";

import {useNavigate, useParams} from 'react-router-dom';

import {isMobile} from 'react-device-detect';

const DeviceTelemetry = () => {
    const navigate = useNavigate();

    const initDevState = {
        id: 0,
        name: "",
        uuid: "",
        status: 0
    };
    const {deviceId} = useParams();
    const [device, setDevice] = useState(initDevState);


    const [telemetry, setTelemetry] = useState([]);
    const telemetryRef = useRef([]);

    const [page, setPage] = useState(1);
    const [count, setCount] = useState(0);
    const [pageSize, setPageSize] = useState(10);

    telemetryRef.current = telemetry;

    const getRequestParams = (deviceId: number, page: number, pageSize: number) => {
        let params: any = {};

        if (deviceId) {
            params["device_id"] = deviceId;
        }

        if (page) {
            params["page"] = page - 1;
        }

        if (pageSize) {
            params["size"] = pageSize;
        }

        return params;
    };

    const findDevice = (id: number) => {
        DeviceRepository.get(id)
            .then((response: any) => {
                setDevice(response.data.data);
            })
            .catch((e) => {
                console.log(e);
            });
    };

    const findAllTelemetry = () => {
        // @ts-ignore
        const params = getRequestParams(deviceId, page, pageSize);

        DeviceTelemetryRepository.findAll(params)
            .then((response) => {
                setTelemetry(response.data.data);
                setCount(Math.ceil(response.data.total / pageSize));
            })
            .catch((e) => {
                console.log(e);
            });
    };
    useEffect(() => {
        findAllTelemetry()

        if (deviceId) {
            // @ts-ignore
            findDevice(deviceId)
        }
    }, [deviceId, page, pageSize]);

    const deleteTelemetry = (rowIndex: any) => {
        // @ts-ignore
        const id = telemetryRef.current[rowIndex].id;

        DeviceTelemetryRepository.remove(id)
            .then((response) => {
                //props.history.push("/registry");

                let newTelemetry = [...telemetryRef.current];
                newTelemetry.splice(rowIndex, 1);

                setTelemetry(newTelemetry);
            })
            .catch((e) => {
                console.log(e);
            });
    };

    const handlePageChange = (event: any, value: number) => {
        setPage(value);
    };

    const columns = useMemo(
        () => [
            {
                Header: "#",
                accessor: "id",
            },
            {
                Header: "Device",
                accessor: "refDevice",
                Cell: (props: any) => {
                    return props.value.name
                },
            },
            {
                Header: "Created At",
                accessor: "createdAt"
            },
            {
                Header: "Telemetry",
                accessor: "json_data",
                Cell: (props: any) => {
                    return JSON.stringify(props.value, null, 2)
                },
            },
            {
                Header: "Actions",
                accessor: "actions",
                Cell: (props) => {
                    const rowIdx = props.row.id;
                    return (
                        <div>
                            {/*<span onClick={() => openTelemetry(rowIdx)}>*/}
                            {/*<i className="far fa-edit action mr-2"></i>*/}
                            {/*</span>*/}
                            <span onClick={() => deleteTelemetry(rowIdx)}>
                            <i className="fas fa-trash action"></i>
                            </span>
                        </div>
                    );
                },
            },
        ],
        []
    );


    let initialState = {}
    if (isMobile) {
        initialState = {hiddenColumns: ['id', "refDevice", "actions"]};
    }

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
        data: telemetry,
        initialState,
    });

    const handleOnRefresh = (e: any) => {
        findAllTelemetry()
    }

    const handleOnClose = (e: any) => {
        navigate(-1)
    }

    return (
        <div>
            <ContentHeader title="Telemetry history"/>
            <section className="content">
                <div className="card">
                    <div className="card-header">
                        <h3 className="card-title">{device.name}</h3>
                    </div>
                    <div className="card-body">
                        <div className="row">
                            <div className="col-md-12">
                                <label htmlFor="deviceUuid">UUID</label>
                                <div className="card-text" id="deviceUuid">{device.uuid}</div>
                            </div>
                        </div>
                        <div className="row">
                            <div className="col-md-10"/>
                            <div className="col-md-2">
                                <div className="btn-group" role="group">
                                    <input className="btn btn-primary" type="button" value="Refresh"
                                           onClick={handleOnRefresh}/>
                                    <input className="btn btn-secondary" type="button" value="Close"
                                           onClick={handleOnClose}/>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div className="card">
                    <div className="card-body">
                        <div className="row">
                            <div className="col-md-12">
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
                                                        <td {...cell.getCellProps()}
                                                            key={id}>{cell.render("Cell")}</td>
                                                    );
                                                })}
                                            </tr>
                                        );
                                    })}
                                    </tbody>
                                </table>
                            </div>
                        </div>
                        <div className="row">
                            <div className="col-md-12 card-tools">
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
            </section>
        </div>
    )
}

export default DeviceTelemetry;