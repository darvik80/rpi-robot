import React from 'react';
import {ContentHeader, DeviceTelemetry} from '@components';

const Telemetry = () => {

    return (
        <div>
            <ContentHeader title="IoT Telemetry"/>
            <section className="content">
                <DeviceTelemetry/>
            </section>
        </div>
    );
};

export default Telemetry;
