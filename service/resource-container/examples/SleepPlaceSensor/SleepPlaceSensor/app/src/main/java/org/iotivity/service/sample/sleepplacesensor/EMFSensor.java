//******************************************************************
//
// Copyright 2015 Euiseok Kim (Seoul National University) All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

package org.iotivity.service.sample.sleepplacesensor;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

import org.iotivity.service.resourcecontainer.AndroidBundleResource;
import org.iotivity.service.resourcecontainer.RcsResourceAttributes;
import org.iotivity.service.resourcecontainer.RcsValue;


/**
 * Created by ikess on 15. 12. 13.
 */
public class EMFSensor extends AndroidBundleResource implements SensorEventListener {
    private static final String LOG_TAG = EMFSensor.class.getSimpleName();
    private final SensorManager mSensorManager;
    private final Sensor emfSensor;

    public EMFSensor(Context context){
        super(context);
        this.setResourceType("oic.r.emf-intensity");
        this.setName("EMFSensor");
        mSensorManager = (SensorManager) context.getApplicationContext().getSystemService(Context.SENSOR_SERVICE);
        emfSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
        mSensorManager.registerListener(this, emfSensor, SensorManager.SENSOR_DELAY_NORMAL);
    }

    public void startListener()
    {
        mSensorManager.registerListener(this, emfSensor, SensorManager.SENSOR_DELAY_NORMAL);
    }

    public void stopListener()
    {
        mSensorManager.unregisterListener(this);
    }

    @Override
    protected void initAttributes() {
        this.m_attributes.put("emf-intensity", 0);
    }

    @Override
    public void handleSetAttributesRequest(RcsResourceAttributes attrs) {
        Log.i(LOG_TAG, "Set Attributes called with ");
        for(String key: attrs.keySet()){
            Log.i(LOG_TAG, " " + key + ": " + attrs.get(key));
        }
    }

    @Override
    public RcsResourceAttributes handleGetAttributesRequest() {
        Log.i(LOG_TAG, "Get Attributes called");
        Log.i(LOG_TAG, "Returning: ");
        for(String key: m_attributes.keySet()){
            Log.i(LOG_TAG, " " + key + ": " + m_attributes.get(key));
        }
        return this.m_attributes;
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {

        double emf = Math.sqrt(sensorEvent.values[0] * sensorEvent.values[0] + sensorEvent.values[1] * sensorEvent.values[1] + sensorEvent.values[2] * sensorEvent.values[2]) * 10;
        Log.i(LOG_TAG, "Sensor event " + emf);
        this.setAttribute("emf-intensity", new RcsValue( emf ) , true);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }
}