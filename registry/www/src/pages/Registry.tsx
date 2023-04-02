import React from 'react';
import {ContentHeader, RegistryList} from '@components';

const Registry = () => {

    return (
        <div>
            <ContentHeader title="IoT Registry" />
            <section className="content">
                <div className="card">
                <RegistryList/>
                </div>
            </section>
        </div>
    );
};

export default Registry;
