import React, {useEffect, useState} from "react";
import {DeviceRepository} from '@repository';
import {useNavigate, useParams} from 'react-router-dom';


const Device = () => {
    const navigate = useNavigate();

    const {id} = useParams();
    const initState = {
        id: 0,
        name: "",
        uuid: "",
        status: 0
    };

    const [device, setDevice] = useState(initState);
    const [updated, setUpdated] = useState(false);

    const findDevice = (id: number) => {
        DeviceRepository.get(id)
            .then((response: any) => {
                setDevice(response.data.data);
            })
            .catch((e) => {
                console.log(e);
            });
    };
    useEffect(() => {
        // @ts-ignore
        findDevice(id)
    }, [id]);

    const handleStatusChange = (e: any) => {
        setDevice(device => {
            return {
                ...device,
                status: parseInt(e.target.value, 10)
            }
        });
        setUpdated(true);
    }

    const handleNameChange = (e: any) => {
        setDevice(device => {
            return {
                ...device,
                name: e.target.value
            }
        });
        setUpdated(true);
    }

    const handleOnApply = (e: any) => {
        if (updated) {
            setUpdated(false);
            DeviceRepository.update(device.id, device)
                .then((response) => {
                })
                .catch((e) => {
                    console.log(e);
                });
            setUpdated(false);
        }
    }

    const handleOnClose = (e: any) => {
        navigate("/devices")
    }

    const statusOptions = [
        {
            label: "active",
            value: 1,
        },
        {
            label: "inactive",
            value: 0,
        }
    ];

    return (
        <div className="card-body">
            <form className="form-horizontal col-md-12">
                <div className="form-group row">
                    <label htmlFor="inputName" className="col-sm-2 col-form-label">Name</label>
                    <div className="col-md-10">
                        <input type="text" className="form-control" id="inputName" placeholder="Name"
                               value={device.name} onChange={handleNameChange}/>
                    </div>
                </div>
                <div className="form-group row">
                    <label htmlFor="inputUuid" className="col-sm-2 col-form-label">UUID</label>
                    <div className="col-md-10">
                        <input type="text" className="form-control" id="inputUuid" placeholder="UUID"
                               value={device.uuid} readOnly={true}/>
                    </div>
                </div>
                <div className="form-group row">
                    <label htmlFor="inputStatus" className="col-sm-2 col-form-label">Status</label>
                    <div className="col-md-10">
                        <select value={device.status} onChange={handleStatusChange} id="inputStatus">
                            {statusOptions.map((option) => (
                                <option key={option.value} id={option.label}
                                        value={option.value}>{option.label}</option>
                            ))}
                        </select>
                    </div>
                </div>
                <div className="row line"/>
                <div className="form-group row ">
                    <div className="col-md-10"/>
                    <div className="btn-group col-md-2" role="group">
                        <input className="btn btn-primary" type="button" disabled={!updated} value="Apply"
                               onClick={handleOnApply}/>
                        <input className="btn btn-secondary" type="button" value="Close" onClick={handleOnClose}/>
                    </div>
                </div>
            </form>
        </div>
    );
};

export default Device;