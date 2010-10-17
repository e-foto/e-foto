#include "EDomValidator.h"

bool EDomValidator::validateProject()
{
    return true;
}

bool EDomValidator::validateHeader(string xml)
{
    EDomElement tester(xml);

    // Procurando elementos que devem existir:
    if (!tester.hasTagName("projectHeader")) return false;
    if (!tester.hasTagName("name")) return false;
    if (!tester.hasTagName("description")) return false;
    if (!tester.hasTagName("filePath")) return false;
    if (!tester.hasTagName("fileName")) return false;
    if (!tester.hasTagName("creation")) return false;
    if (!tester.hasTagName("modification")) return false;
    if (!tester.hasTagName("owner")) return false;
    if (!tester.hasTagName("aims")) return false;
    if (!tester.hasTagName("context")) return false;

    // Testando elementos que não podem ter conteudo vazio:
    if (tester.elementByTagName("name").toString() == "") return false;
    if (tester.elementByTagName("filePath").toString() == "") return false;
    if (tester.elementByTagName("fileName").toString() == "") return false;
    if (tester.elementByTagName("creation").toString() == "") return false;
    if (tester.elementByTagName("modification").toString() == "") return false;

    return true;
}

bool EDomValidator::validateTerrain(string xml)
{
    EDomElement tester(xml);

    // Procurando elementos que devem existir:
    if (!tester.hasTagName("terrain")) return false;
    if (!tester.hasTagName("meanAltitude")) return false;
    if (!tester.hasTagName("minAltitude")) return false;
    if (!tester.hasTagName("maxAltitude")) return false;
    if (!tester.hasTagName("GRS")) return false;
    if (!tester.hasTagName("CPS")) return false;
    if (!tester.hasTagName("workAreaCenterCoordinates")) return false;
    {
        EDomElement workAreaTester = tester.elementByTagName("workAreaCenterCoordinates");
        if (!workAreaTester.hasTagName("Lat")) return false;
        {
            EDomElement latTester = workAreaTester.elementByTagName("Lat");
            if (!latTester.hasTagName("degrees")) return false;
            if (!latTester.hasTagName("minutes")) return false;
            if (!latTester.hasTagName("seconds")) return false;
        }
        if (!workAreaTester.hasTagName("Long")) return false;
        {
            EDomElement longTester = workAreaTester.elementByTagName("Long");
            if (!longTester.hasTagName("degrees")) return false;
            if (!longTester.hasTagName("minutes")) return false;
            if (!longTester.hasTagName("seconds")) return false;
        }
        if (!workAreaTester.hasTagName("utmFuse")) return false;
    }

    // Testando elementos que não podem ter conteudo vazio:
    if (tester.elementByTagName("meanAltitude").toString() == "") return false;
    if (tester.elementByTagName("minAltitude").toString() == "") return false;
    if (tester.elementByTagName("maxAltitude").toString() == "") return false;
    if (tester.elementByTagName("GRS").toString() == "") return false;
    if (tester.elementByTagName("CPS").toString() == "") return false;
    {
        EDomElement workAreaTester = tester.elementByTagName("workAreaCenterCoordinates");
        {
            EDomElement latTester = workAreaTester.elementByTagName("Lat");
            if (latTester.elementByTagName("degrees").toString() == "") return false;
            if (latTester.elementByTagName("minutes").toString() == "") return false;
            if (latTester.elementByTagName("seconds").toString() == "") return false;
        }
        {
            EDomElement longTester = workAreaTester.elementByTagName("Long");
            if (longTester.elementByTagName("degrees").toString() == "") return false;
            if (longTester.elementByTagName("minutes").toString() == "") return false;
            if (longTester.elementByTagName("seconds").toString() == "") return false;
        }
        if (workAreaTester.elementByTagName("utmFuse").toString() == "") return false;
    }

    return true;
}

bool EDomValidator::validateFlight(string xml)
{

    EDomElement tester(xml);

    // Procurando elementos que devem existir:
    if (!tester.hasTagName("flight")) return false;
    if (!tester.hasTagName("flightId")) return false;
    if (!tester.hasTagName("description")) return false;
    if (!tester.hasTagName("execution")) return false;
    if (!tester.hasTagName("producerName")) return false;
    if (!tester.hasTagName("nominalScale")) return false;
    {
        EDomElement scaleTester = tester.elementByTagName("nominalScale");
        if (!scaleTester.hasTagName("mml:mfrac")) return false;
        {
            deque<EDomElement> mnTester = scaleTester.elementsByTagName("mml:mn");
            if (mnTester.size() != 2) return false;
        }
    }
    if (!tester.hasTagName("flightHeight")) return false;
    if (!tester.hasTagName("overlap")) return false;
    {
        EDomElement overlapTester = tester.elementByTagName("overlap");
        if (!overlapTester.hasTagName("longitudinal")) return false;
        if (!overlapTester.hasTagName("transversal")) return false;
    }

    // Testando elementos que não podem ter conteudo vazio:
    if (tester.elementByTagName("flightId").toString() == "") return false;
    if (tester.elementByTagName("execution").toString() == "") return false;
    {
        EDomElement scaleTester = tester.elementByTagName("nominalScale");
        {
            deque<EDomElement> mnTester = scaleTester.elementsByTagName("mml:mn");
            if (mnTester.at(0).toString() == "") return false;
            if (mnTester.at(1).toString() == "") return false;
        }
    }
    if (tester.elementByTagName("flightHeight").toString() == "") return false;
    {
        EDomElement overlapTester = tester.elementByTagName("overlap");
        if (overlapTester.elementByTagName("longitudinal").toString() == "") return false;
        if (overlapTester.elementByTagName("transversal").toString() == "") return false;
    }

    return true;
}

bool EDomValidator::validateSensor(string xml)
{
    EDomElement tester(xml);

    // Procurando elementos que devem existir:
    if (!tester.hasTagName("sensor")) return false;
    if (!tester.hasTagName("type")) return false;
    {
        EDomElement typeTester = tester.elementByTagName("type");
        if (!typeTester.hasTagName("geometry")) return false;
        if (!typeTester.hasTagName("platform")) return false;
        if (!typeTester.hasTagName("detector")) return false;
        if (!typeTester.hasTagName("energySource")) return false;
        if (!typeTester.hasTagName("spectralRanges")) return false;
        {
            EDomElement spectralTester = typeTester.elementByTagName("spectralRanges");
            if (!spectralTester.hasTagName("spectralRange")) return false;
            {
                EDomElement rangeTester = spectralTester.elementByTagName("spectralRange");
                if (!rangeTester.hasTagName("inferiorLimit")) return false;
                if (!rangeTester.hasTagName("superiorLimit")) return false;
            }
        }
    }
    if (!tester.hasTagName("description")) return false;
    if (!tester.hasTagName("calibrationCertificate")) return false;
    {
        EDomElement calibrationTester = tester.elementByTagName("calibrationCertificate");
        if (!calibrationTester.hasTagName("number")) return false;
        if (!calibrationTester.hasTagName("dispatch")) return false;
        if (!calibrationTester.hasTagName("expiration")) return false;
    }
    if (!tester.hasTagName("focalDistance")) return false;
    {
        EDomElement focalTester = tester.elementByTagName("focalDistance");
        if (!focalTester.hasTagName("value")) return false;
        if (!focalTester.hasTagName("sigma")) return false;
    }
    if (!tester.hasTagName("distortionCoefficients")) return false;
    {
        EDomElement distortionTester = tester.elementByTagName("distortionCoefficients");
        if (!distortionTester.hasTagName("radialSymmetric")) return false;
        {
            EDomElement radialTester = distortionTester.elementByTagName("radialSymmetric");
            if (!radialTester.hasTagName("k0")) return false;
            {
                EDomElement k0Tester = radialTester.elementByTagName("k0");
                if (!k0Tester.hasTagName("value")) return false;
                if (!k0Tester.hasTagName("sigma")) return false;
            }
            if (!radialTester.hasTagName("k1")) return false;
            {
                EDomElement k1Tester = radialTester.elementByTagName("k1");
                if (!k1Tester.hasTagName("value")) return false;
                if (!k1Tester.hasTagName("sigma")) return false;
            }
            if (!radialTester.hasTagName("k2")) return false;
            {
                EDomElement k2Tester = radialTester.elementByTagName("k2");
                if (!k2Tester.hasTagName("value")) return false;
                if (!k2Tester.hasTagName("sigma")) return false;
            }
            if (!radialTester.hasTagName("k3")) return false;
            {
                EDomElement k3Tester = radialTester.elementByTagName("k3");
                if (!k3Tester.hasTagName("value")) return false;
                if (!k3Tester.hasTagName("sigma")) return false;
            }
        }
        if (!distortionTester.hasTagName("decentered")) return false;
        {
            EDomElement decenteredTester = distortionTester.elementByTagName("decentered");
            if (!decenteredTester.hasTagName("P1")) return false;
            {
                EDomElement P1Tester = decenteredTester.elementByTagName("P1");
                if (!P1Tester.hasTagName("value")) return false;
                if (!P1Tester.hasTagName("sigma")) return false;
            }
            if (!decenteredTester.hasTagName("P2")) return false;
            {
                EDomElement P2Tester = decenteredTester.elementByTagName("P2");
                if (!P2Tester.hasTagName("value")) return false;
                if (!P2Tester.hasTagName("sigma")) return false;
            }
        }
    }
    if (!tester.hasTagName("principalPointCoordinates")) return false;
    {
        EDomElement principalTester = tester.elementByTagName("principalPointCoordinates");
        if (!principalTester.hasTagName("gml:pos")) return false;
        if (!principalTester.hasTagName("sigma")) return false;
    }

    // Testando elementos que não podem ter conteudo vazio:
    {
        EDomElement typeTester = tester.elementByTagName("type");
        if (typeTester.elementByTagName("geometry").toString() == "") return false;
        if (typeTester.elementByTagName("platform").toString() == "") return false;
        if (typeTester.elementByTagName("detector").toString() == "") return false;
    }
    {
        EDomElement calibrationTester = tester.elementByTagName("calibrationCertificate");
        if (calibrationTester.elementByTagName("number").toString() == "") return false;
    }
    {
        EDomElement focalTester = tester.elementByTagName("focalDistance");
        if (focalTester.elementByTagName("value").toString() == "") return false;
    }
    {
        EDomElement principalTester = tester.elementByTagName("principalPointCoordinates");
        if (principalTester.elementByTagName("gml:pos").toString() == "") return false;
    }

    return true;
}

bool EDomValidator::validateImage(string xml)
{
    EDomElement tester(xml);

    // Procurando elementos que devem existir:
    if (!tester.hasTagName("image")) return false;
    if (!tester.hasTagName("img_id")) return false;
    if (!tester.hasTagName("width")) return false;
    if (!tester.hasTagName("height")) return false;
    if (!tester.hasTagName("fileName")) return false;
    if (!tester.hasTagName("filePath")) return false;
    if (!tester.hasTagName("flightId")) return false;
    if (!tester.hasTagName("resolution")) return false;
    if (!tester.hasTagName("spatialCoordinates")) return false;
    {
        EDomElement spatialTester = tester.elementByTagName("spatialCoordinates");
        if (!spatialTester.hasTagName("gml:pos")) return false;
        if (!spatialTester.hasTagName("sigma")) return false;
    }

    // Testando elementos que não podem ter conteudo vazio:
    if (tester.elementByTagName("width").toString() == "") return false;
    if (tester.elementByTagName("height").toString() == "") return false;
    if (tester.elementByTagName("fileName").toString() == "") return false;
    if (tester.elementByTagName("filePath").toString() == "") return false;
    if (tester.elementByTagName("resolution").toString() == "") return false;

    return true;
}

bool EDomValidator::validatePoint(string xml)
{
    EDomElement tester(xml);

    // Procurando elementos que devem existir:
    if (!tester.hasTagName("point")) return false;
    if (!tester.hasTagName("gcp_id")) return false;
    if (!tester.hasTagName("description")) return false;
    if (!tester.hasTagName("spatialCoordinates")) return false;
    {
        EDomElement spatialTester = tester.elementByTagName("spatialCoordinates");
        if (!spatialTester.hasTagName("gml:pos")) return false;
        if (!spatialTester.hasTagName("sigma")) return false;
    }

    // Testando elementos que não podem ter conteudo vazio:
    {
        EDomElement spatialTester = tester.elementByTagName("spatialCoordinates");
        if (spatialTester.elementByTagName("gml:pos").toString() == "") return false;
        if (spatialTester.elementByTagName("sigma").toString() == "") return false;
    }

    return true;
}
