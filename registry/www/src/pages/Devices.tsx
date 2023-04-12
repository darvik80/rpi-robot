import React from 'react';
import {ContentHeader, DeviceList} from '@components';

const RegistryDo = () => {

    return (
        <div>
            <ContentHeader title="IoT Devices" />
            <section className="content">
                <div className="card">
                <DeviceList/>
                </div>
            </section>
        </div>
    );
};

export default RegistryDo;
