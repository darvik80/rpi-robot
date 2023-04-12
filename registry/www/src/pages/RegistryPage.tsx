import React from 'react';
import {ContentHeader, RegistryList} from '@components';

const RegistryPage = () => {

    return (
        <div>
            <ContentHeader title="IoT Registry"/>
            <section className="content">
                <RegistryList/>
            </section>
        </div>
    );
};

export default RegistryPage;
