import React, {useEffect, useMemo, useRef, useState} from "react";
import DeviceTelemetryRepository from "../../repository/DeviceTelemetryRepository";
import {useTable} from "react-table";
import Pagination from "@material-ui/lab/Pagination";


const DeviceTelemetry = () => {
    const [telemetry, setTelemetry] = useState([]);
    const telemetryRef = useRef([]);

    const [page, setPage] = useState(1);
    const [deviceId, setDeviceId] = useState(0);
    const [count, setCount] = useState(0);
    const [pageSize, setPageSize] = useState(20);

    telemetryRef.current = telemetry;

    const getRequestParams = (deviceId: number, page: number, pageSize: number) => {
        let params: any = {};

        if (deviceId) {
            params["deviceId"] = deviceId;
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
    useEffect(findAllRegistries, [page, pageSize]);

    const openTelemetry = (rowIndex: any) => {
        // @ts-ignore
        const id = telemetryRef.current[rowIndex].id;

        //props.history.push("/registry/" + id);
    };

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
    });

    return (
        <div className="list row">
            <div className="col-md-12 list">
                <div className="card-header">
                    <h3 className="card-title">Registries</h3>
                </div>
                <div className="card-body">
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
                </div>
            </div>
            <div className="col-md-10">
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
    );
};

export default DeviceTelemetry;