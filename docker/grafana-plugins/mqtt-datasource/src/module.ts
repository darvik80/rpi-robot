import {DataSourcePlugin} from '@grafana/data';
import {DataSource} from './datasource';
import {ConfigEditor} from './ConfigEditor';
import {QueryEditor} from './QueryEditor';
import {MqttDataSourceOptions, MqttQuery} from './types';

export const plugin = new DataSourcePlugin<DataSource, MqttQuery, MqttDataSourceOptions>(DataSource)
  .setConfigEditor(ConfigEditor)
  .setQueryEditor(QueryEditor);
