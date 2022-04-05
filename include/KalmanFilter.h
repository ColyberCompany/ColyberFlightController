/**
 * @file KalmanFilter.h
 * @author Aleksy Walczak (aleksyww@gmail.com)
 * @brief 
 * @date 2021-08-20
 * 
 */

#ifndef KALMANFILTER_H
#define KALMANFILTER_H


class KalmanFilter
{
    float altitudeEstimated;                
    float velocityEstimated;                
    float accelerationEstimated;            
                                            
    float altitudeVarianceEstimated;
    float velocityVarianceEstimated;
    float accelerationVarianceEstimated;      

    float altitudePredicted;                
    float velocityPredicted;                
    float accelerationPredicted;            
                                            
    float altitudeVariancePredicted;
    float velocityVariancePredicted;
    float accelerationVariancePredicted;      

    float altitudeKalmanGain;
    float accelerationKalmanGain;

    float altitudeMeasured;
    float accelerationMeasured;

    float altitudeMeasureError;
    float accelerationMeasureError;

    float altitudeFilteringVariance;
    float velocityFilteringVariance;
    float accelerationFilteringVariance;

    float deltaT;
    float deltaT2;
    float deltaT2Half;
    float deltaT4Forth;

public:
/**
 * @brief 
 * 
 * @param altitudeMeasureError 
 * @param accelerationMeasureError 
 * @param altitudeFilteringVariance 
 * @param velocityFilteringVariance 
 * @param accelerationFilteringVariance 
 * @param deltaT 
 */
    KalmanFilter(float altitudeMeasureError, float accelerationMeasureError,
        float altitudeFilteringVariance, float velocityFilteringVariance,float accelerationFilteringVariance,
        float deltaT) :
        altitudeVariancePredicted(altitudeFilteringVariance),
        velocityVariancePredicted(velocityFilteringVariance),
        accelerationVariancePredicted(accelerationFilteringVariance),

        altitudeFilteringVariance(altitudeFilteringVariance),
        velocityFilteringVariance(velocityFilteringVariance),
        accelerationFilteringVariance(accelerationFilteringVariance),

        altitudeMeasureError(altitudeMeasureError),
        accelerationMeasureError(accelerationMeasureError),
        
        deltaT(deltaT)
        {
            deltaT2 = deltaT * deltaT;
            deltaT2Half = deltaT2 / 2;
            deltaT4Forth = deltaT2Half * deltaT2Half;
        }

    float update(float measuredAltitude_m, float measuredAcceleration_mps2)
    {
        altitudeMeasured = measuredAltitude_m;
        accelerationMeasured = measuredAcceleration_mps2;

        altitudeEstimated = altitudePredicted + altitudeKalmanGain * (altitudeMeasured - altitudePredicted);
        velocityEstimated = velocityPredicted;
        accelerationEstimated = accelerationPredicted + accelerationKalmanGain * (accelerationMeasured - accelerationPredicted);

        altitudeVarianceEstimated = (1 - altitudeKalmanGain) * altitudeVariancePredicted;
        velocityVarianceEstimated = velocityVariancePredicted;
        accelerationVarianceEstimated = (1 - accelerationKalmanGain) * accelerationVariancePredicted;

        altitudePredicted = altitudeEstimated + deltaT * velocityEstimated + deltaT2Half * accelerationEstimated;
        velocityPredicted = velocityEstimated + deltaT * accelerationEstimated;
        accelerationPredicted = accelerationEstimated;

        altitudeVariancePredicted = altitudeVarianceEstimated + deltaT2 * velocityVarianceEstimated + deltaT4Forth * accelerationVarianceEstimated + altitudeFilteringVariance;
        velocityVariancePredicted = velocityVarianceEstimated + deltaT2 * accelerationVarianceEstimated + velocityFilteringVariance;
        accelerationVariancePredicted = accelerationVarianceEstimated + accelerationFilteringVariance;

        altitudeKalmanGain = altitudeVariancePredicted / (altitudeVariancePredicted + altitudeMeasureError);
        accelerationKalmanGain = accelerationVariancePredicted / (accelerationVariancePredicted + accelerationMeasureError);

        return altitudeEstimated;
    }
    
    void reset()
    {
        altitudeEstimated = 0;                
        velocityEstimated = 0;                
        accelerationEstimated = 0;                                             
        altitudeVarianceEstimated = 0;
        velocityVarianceEstimated = 0;
        accelerationVarianceEstimated = 0; 
        altitudePredicted = 0;                
        velocityPredicted = 0;                
        accelerationPredicted = 0;                                             
        altitudeVariancePredicted = altitudeFilteringVariance;
        velocityVariancePredicted = velocityFilteringVariance;
        accelerationVariancePredicted = accelerationFilteringVariance; 
        altitudeKalmanGain = 0;
        accelerationKalmanGain = 0;
    }
};


#endif
