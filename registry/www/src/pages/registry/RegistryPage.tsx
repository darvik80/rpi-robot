import React, {useEffect, useMemo, useRef, useState} from "react";

import {useNavigate} from "react-router-dom";
import {RegistryRepository} from "@repository";
import {useTable} from "react-table";
import Pagination from "@material-ui/lab/Pagination";
import {ContentHeader} from "@components";


const RegistryPage = () => {
    const navigate = useNavigate();

    const [registries, setRegistries] = useState([]);
    const [searchName, setSearchName] = useState("");
    const registriesRef = useRef([]);

    const [page, setPage] = useState(1);
    const [count, setCount] = useState(0);
    const [pageSize, setPageSize] = useState(10);

    registriesRef.current = registries;

    const onChangeSearchName = (e: any) => {
        const searchName = e.target.value;
        setSearchName(searchName);
    };

    const getRequestParams = (searchName: string, page: number, pageSize: number) => {
        let params: any = {};

        if (searchName) {
            params["name"] = searchName;
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
        const params = getRequestParams(searchName, page, pageSize);

        RegistryRepository.findAll(params)
            .then((response) => {
                setRegistries(response.data.data);
                setCount(Math.ceil(response.data.total / pageSize));
            })
            .catch((e) => {
                console.log(e);
            });
    };
    const findByName = () => {
        setPage(1);
        findAllRegistries();
    }

    useEffect(findAllRegistries, [page, pageSize]);

    const openRegistry = (rowIndex: any) => {
        // @ts-ignore
        const id = registriesRef.current[rowIndex].id;

        navigate("/registries/" + id);
    };

    const deleteRegistry = (rowIndex: any) => {
        // @ts-ignore
        const id = registriesRef.current[rowIndex].id;

        RegistryRepository.remove(id)
            .then((response) => {
                //props.history.push("/registry");

                let newRegistries = [...registriesRef.current];
                newRegistries.splice(rowIndex, 1);

                setRegistries(newRegistries);
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
                            <span onClick={() => openRegistry(rowIdx)}>
                            <i className="far fa-edit action mr-2"></i>
                            </span>

                            <span onClick={() => deleteRegistry(rowIdx)}>
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
        data: registries,
    });

    return (
        <div>
            <ContentHeader title="Registries"/>
            <section className="content">
                <div className="card">
                    <div className="card-body">
                        <div className="row">
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
                        </div>
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
        ;
};

export default RegistryPage;