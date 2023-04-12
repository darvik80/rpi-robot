import React from 'react';
import {ContentHeader, RegistryList} from '@components';

const RegistryPage = () => {

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

export default RegistryPage;
