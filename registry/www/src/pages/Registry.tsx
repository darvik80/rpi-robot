import React from 'react';
import {ContentHeader, Registry} from '@components';

const RegistryPage = () => {

    return (
        <div>
            <ContentHeader title="IoT Registry"/>
            <section className="content">
                <div className="card">
                    <Registry/>
                </div>
            </section>
        </div>
    );
};

export default RegistryPage;
