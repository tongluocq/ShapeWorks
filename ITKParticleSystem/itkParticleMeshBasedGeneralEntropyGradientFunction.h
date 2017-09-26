#ifndef ITKPARTICLEMESHBASEDGENERALENTROPYGRADIENTFUNCTION_H
#define ITKPARTICLEMESHBASEDGENERALENTROPYGRADIENTFUNCTION_H

/*
 * Author: Praful
 * Class for mesh based features/attributes for shape correspondence
 */

#include "itkParticleVectorFunction.h"
#include <vector>
#include <numeric>
namespace itk
{

template <unsigned int VDimension>
class ParticleMeshBasedGeneralEntropyGradientFunction : public ParticleVectorFunction<VDimension>
{
public:
    /** Standard class typedefs. */
    typedef ParticleMeshBasedGeneralEntropyGradientFunction Self;
    typedef SmartPointer<Self>  Pointer;
    typedef SmartPointer<const Self>  ConstPointer;
    typedef ParticleVectorFunction<VDimension> Superclass;
    itkTypeMacro( ParticleMeshBasedGeneralEntropyGradientFunction, ParticleVectorFunction)

    /** Type of particle system. */
    typedef typename Superclass::ParticleSystemType ParticleSystemType;

    /** Vector & Point types. */
    typedef typename Superclass::VectorType VectorType;
    typedef typename ParticleSystemType::PointType PointType;
    typedef vnl_vector<float> vnl_vector_type;
    typedef vnl_matrix<float> vnl_matrix_type;

    /** Method for creation through the object factory. */
    itkNewMacro(Self)

    /** Dimensionality of the domain of the particle system. */
    itkStaticConstMacro(Dimension, unsigned int, VDimension);

    /** The first argument is a pointer to the particle system.  The second
         argument is the index of the domain within that particle system.  The
         third argument is the index of the particle location within the given
         domain. */

    virtual VectorType Evaluate(unsigned int, unsigned int,
                                const ParticleSystemType *, double &, double&) const;

    virtual VectorType Evaluate(unsigned int a,  unsigned int b,
                                const ParticleSystemType *c, double &d) const
    {
        double e;
        return this->Evaluate(a,b,c,d,e);
    }


    virtual double Energy(unsigned int a, unsigned int b,
                          const ParticleSystemType *c) const
    {
        double e, d;
        this->Evaluate(a,b,c,d,e);
        return e;
    }

    vnl_matrix_type GetYMatrix(const ParticleSystemType *c) const;

    void SetDimensions()
    {
        num_samples = this->m_ParticleSystem->GetNumberOfDomains() / m_DomainsPerShape;

        int ndims = 0;
        for (int i = 0; i < m_DomainsPerShape; i++)
        {
            int shapeAttrs = m_AttributesPerDomain[i];
            if (m_UseXYZ[i])
                shapeAttrs += 3;
            if (m_UseNormals[i])
                shapeAttrs += 3;
            ndims += this->m_ParticleSystem->GetNumberOfParticles(i) * shapeAttrs;
        }

        num_dims = ndims;
    }

    /** Called before each iteration of a solver. */
    virtual void BeforeIteration()
    {
        this->SetDimensions();
        this->ComputeUpdates();
    }


    /** Called after each iteration of the solver. */
    virtual void AfterIteration()
    {
        // Update the annealing parameter.
        if (m_HoldMinimumVariance != true && !m_UseMeanEnergy)
        {
            m_Counter++;
            if (m_Counter >=  m_RecomputeCovarianceInterval)
            {
                m_Counter = 0;
                m_MinimumVariance *= m_MinimumVarianceDecayConstant;
            }
        }
    }
    void SetMinimumVarianceDecay(double initial_value, double final_value, double time_period)
    {
        m_MinimumVarianceDecayConstant =  exp(log(final_value / initial_value) / time_period);
        m_MinimumVariance = initial_value;
        m_HoldMinimumVariance = false;
    }

    void SetMinimumVariance( double d)
    { m_MinimumVariance = d;}
    double GetMinimumVariance() const
    { return m_MinimumVariance; }

    bool GetHoldMinimumVariance() const
    { return m_HoldMinimumVariance; }
    void SetHoldMinimumVariance(bool b)
    { m_HoldMinimumVariance = b; }

    void SetRecomputeCovarianceInterval(int i)
    { m_RecomputeCovarianceInterval = i; }
    int GetRecomputeCovarianceInterval() const
    { return m_RecomputeCovarianceInterval; }

    void SetAttributeScales( const std::vector<double> &s)
    { m_AttributeScales = s; }

    /** Set/Get the number of domains per shape. */
    void SetDomainsPerShape(int i)
    { m_DomainsPerShape = i; }
    int GetDomainsPerShape() const
    { return m_DomainsPerShape; }

    void SetAttributesPerDomain(const std::vector<int> &i)
    { m_AttributesPerDomain = i; }

    void UseMeanEnergy()
    { m_UseMeanEnergy = true; }
    void UseEntropy()
    { m_UseMeanEnergy = false; }

    void SetXYZ(int i, bool val)
    {
        if (m_UseXYZ.size() != m_DomainsPerShape)
            m_UseXYZ.resize(m_DomainsPerShape);
        m_UseXYZ[i] = val;
    }
    void SetNormals(int i, bool val)
    {
        if (m_UseNormals.size() != m_DomainsPerShape)
            m_UseNormals.resize(m_DomainsPerShape);
        m_UseNormals[i] = val;
    }

    virtual typename ParticleVectorFunction<VDimension>::Pointer Clone()
    {
        typename ParticleMeshBasedGeneralEntropyGradientFunction<VDimension>::Pointer copy =
                ParticleMeshBasedGeneralEntropyGradientFunction<VDimension>::New();

        // from itkParticleVectorFunction
        copy->m_DomainNumber = this->m_DomainNumber;
        copy->m_ParticleSystem = this->m_ParticleSystem;

        // local
        copy->m_AttributeScales = this->m_AttributeScales;
        copy->m_Counter = this->m_Counter;
        copy->m_CurrentEnergy = this->m_CurrentEnergy;
        copy->m_HoldMinimumVariance = this->m_HoldMinimumVariance;
        copy->m_MinimumEigenValue  = this->m_MinimumEigenValue;
        copy->m_MinimumVariance = this->m_MinimumVariance;
        copy->m_MinimumVarianceDecayConstant = this->m_MinimumVarianceDecayConstant;
        copy->m_PointsUpdate = this->m_PointsUpdate;
        copy->m_RecomputeCovarianceInterval = this->m_RecomputeCovarianceInterval;
        copy->m_AttributesPerDomain = this->m_AttributesPerDomain;
        copy->m_DomainsPerShape = this->m_DomainsPerShape;
        copy->m_UseMeanEnergy = this->m_UseMeanEnergy;
        copy->m_mean = this->m_mean;
        copy->m_UseNormals = this->m_UseNormals;
        copy->m_UseXYZ = this->m_UseXYZ;
        return (typename ParticleVectorFunction<VDimension>::Pointer)copy;
    }

protected:
    ParticleMeshBasedGeneralEntropyGradientFunction()
    {
        // m_MinimumVarianceBase = 1.0;//exp(log(1.0e-5)/10000.0);
        m_HoldMinimumVariance = true;
        m_MinimumVariance = 1.0e-5;
        m_MinimumEigenValue = 0.0;
        m_MinimumVarianceDecayConstant = log(2.0) / 50000.0;
        m_RecomputeCovarianceInterval = 5;
        m_Counter = 0;
        m_PointsUpdate.clear();
        m_UseMeanEnergy = true;
        m_UseNormals.clear();
        m_UseXYZ.clear();
        m_mean.clear();
        num_dims = 0;
        num_samples = 0;
    }
    virtual ~ParticleMeshBasedGeneralEntropyGradientFunction() {}
    void operator=(const ParticleMeshBasedGeneralEntropyGradientFunction &);
    ParticleMeshBasedGeneralEntropyGradientFunction(const ParticleMeshBasedGeneralEntropyGradientFunction &);

    virtual void ComputeUpdates();
    vnl_matrix_type m_PointsUpdate;

    double m_MinimumVariance;
    double m_MinimumEigenValue;
    //  double m_MinimumVarianceBase;
    bool m_HoldMinimumVariance;
    int m_RecomputeCovarianceInterval;
    double m_MinimumVarianceDecayConstant;
    int m_Counter;
    std::vector<double> m_AttributeScales; //size \sum_i n_i
    int m_DomainsPerShape;
    std::vector<int> m_AttributesPerDomain; // n
    double m_CurrentEnergy;
    bool m_UseMeanEnergy;
    std::vector<bool> m_UseXYZ;
    std::vector<bool> m_UseNormals;
    vnl_vector_type m_mean;
    int num_dims, num_samples;

};
} // end namespace

#if ITK_TEMPLATE_EXPLICIT
#include "Templates/itkParticleMeshBasedGeneralEntropyGradientFunction+-.h"
#endif

#if ITK_TEMPLATE_TXX
#include "itkParticleMeshBasedGeneralEntropyGradientFunction.txx"
#endif

#endif // ITKPARTICLEMESHBASEDGENERALENTROPYGRADIENTFUNCTION_H