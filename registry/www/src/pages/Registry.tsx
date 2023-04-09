import React from 'react';
import {ContentHeader, RegistryList} from '@components';

const RegistryDo = () => {

    return (
        <div>
            <ContentHeader title="IoT RegistryDo" />
            <section className="content">
                <div className="card">
                <RegistryList/>
                </div>
            </section>
        </div>
    );
};

export default RegistryDo;
