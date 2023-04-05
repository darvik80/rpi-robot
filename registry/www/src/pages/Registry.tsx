import React from 'react';
import {ContentHeader, RegistryList} from '@components';

const IotRegistry = () => {

    return (
        <div>
            <ContentHeader title="IoT IotRegistry" />
            <section className="content">
                <div className="card">
                <RegistryList/>
                </div>
            </section>
        </div>
    );
};

export default IotRegistry;
