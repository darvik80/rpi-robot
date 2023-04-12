import React from 'react';
import {ContentHeader, DeviceList} from '@components';

const Devices = () => {

    return (
        <div>
            <ContentHeader title="IoT Devices"/>
            <section className="content">
                <DeviceList/>
            </section>
        </div>
    );
};

export default Devices;
