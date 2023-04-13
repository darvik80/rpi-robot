import React from 'react';
import {ContentHeader} from '@components';

const Dashboard = () => {
    return (
        <div>
            <ContentHeader title="Dashboard"/>

            <section className="content">
                <div className="container-fluid">
                    <div className="row">
                        <div className="col-lg-3 col-6">
                            <div className="small-box bg-info">
                                <div className="inner">
                                    <h3>Registries</h3>
                                </div>
                                <div className="icon">
                                    <i className="ion ion-bag"/>
                                </div>
                                <a href="/registries" className="small-box-footer">
                                    More info <i className="fas fa-arrow-circle-right"/>
                                </a>
                            </div>
                        </div>
                        <div className="col-lg-3 col-6">
                            <div className="small-box bg-success">
                                <div className="inner">
                                    <h3>
                                        Devices
                                    </h3>
                                </div>
                                <div className="icon">
                                    <i className="ion ion-stats-bars"/>
                                </div>
                                <a href="/devices" className="small-box-footer">
                                    More info <i className="fas fa-arrow-circle-right"/>
                                </a>
                            </div>
                        </div>
                        {/*<div className="col-lg-3 col-6">*/}
                        {/*  <div className="small-box bg-warning">*/}
                        {/*    <div className="inner">*/}
                        {/*      <h3>44</h3>*/}

                        {/*      <p>User Registrations</p>*/}
                        {/*    </div>*/}
                        {/*    <div className="icon">*/}
                        {/*      <i className="ion ion-person-add" />*/}
                        {/*    </div>*/}
                        {/*    <a href="/" className="small-box-footer">*/}
                        {/*      More info <i className="fas fa-arrow-circle-right" />*/}
                        {/*    </a>*/}
                        {/*  </div>*/}
                        {/*</div>*/}
                        {/*<div className="col-lg-3 col-6">*/}
                        {/*  <div className="small-box bg-danger">*/}
                        {/*    <div className="inner">*/}
                        {/*      <h3>65</h3>*/}

                        {/*      <p>Unique Visitors</p>*/}
                        {/*    </div>*/}
                        {/*    <div className="icon">*/}
                        {/*      <i className="ion ion-pie-graph" />*/}
                        {/*    </div>*/}
                        {/*    <a href="/" className="small-box-footer">*/}
                        {/*      More info <i className="fas fa-arrow-circle-right" />*/}
                        {/*    </a>*/}
                        {/*  </div>*/}
                        {/*</div>*/}
                    </div>
                </div>
            </section>
        </div>
    );
};

export default Dashboard;
