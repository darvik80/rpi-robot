import React from 'react';
import {ContentHeader, Registry} from '@components';

const RegistryPage = () => {

    return (
        <div>
            <ContentHeader title="IoT Registry"/>
            <section className="content">
                <Registry/>
            </section>
        </div>
    );
};

export default RegistryPage;
