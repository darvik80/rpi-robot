import React from 'react';
import {ContentHeader, DeviceTelemetry} from '@components';

const RegistryDo = () => {

    return (
        <div>
            <ContentHeader title="IoT Devices"/>
            <section className="content">
                <div className="card">
                    <DeviceTelemetry/>
                </div>
            </section>
        </div>
    );
};

export default RegistryDo;
