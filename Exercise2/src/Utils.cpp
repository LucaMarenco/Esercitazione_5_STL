#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}

bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;

    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

		string token;
        unsigned int id;
        unsigned int marker;
		double x;
		double y;

		getline(converter, token, ';');
		id = stoi(token);

		getline(converter, token, ';');
		marker = stoul(token);

		getline(converter, token, ';');
		x = stod(token);

		getline(converter, token, ';');
		y = stod(token);

		
		mesh.Cell0DsCoordinates(0, id) = x;
		mesh.Cell0DsCoordinates(1, id) = y;
        

        mesh.Cell0DsId.push_back(id);
		
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
        }
    }
    return true;
}

bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // Remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);
		
        string token;
		
        unsigned int id;
        unsigned int marker;
		unsigned int origin;
        unsigned int end;
		
        Vector2i vertices;

        
		getline(converter, token, ';');
        id = stoi(token);

        getline(converter, token, ';');
        marker = stod(token);

        getline(converter, token, ';');
        origin = stoi(token);

        getline(converter, token, ';');
        end = stoi(token);

        
		mesh.Cell1DsId.push_back(id);
        mesh.Cell1DsExtrema(0, id) = origin;
        mesh.Cell1DsExtrema(1, id) = end;

		
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
        }
    }

    return true;
}


bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

	// Remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);
		
		string token;

        unsigned int id;
		unsigned int marker;
        size_t numVertices;
		size_t numEdges;

        // ID
        getline(converter, token, ';');
        id = stoi(token);

        // Marker
        getline(converter, token, ';');
        marker = stoi(token);

        // Numero di vertici
        getline(converter, token, ';');
        numVertices = static_cast<size_t>(stoi(token));

        // Vertici
        vector<unsigned int> vertices(numVertices);
        for (size_t i = 0; i < numVertices; ++i)
        {
            getline(converter, token, ';');
            vertices[i] = static_cast<unsigned int>(stoi(token));
        }

        // Numero di spigoli
        getline(converter, token, ';');
        numEdges = static_cast<size_t>(stoi(token));

        // Spigoli
        vector<unsigned int> edges(numEdges);
        for (size_t i = 0; i < numEdges; ++i)
        {
            getline(converter, token, ';');
            edges[i] = static_cast<unsigned int>(stoi(token));
        }

    
        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
		
		if(marker != 0)
        {
            const auto it = mesh.MarkerCell2Ds.find(marker);
            if(it == mesh.MarkerCell2Ds.end())
            {
                mesh.MarkerCell2Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
        }
		
    }

    return true;
}

bool TestEdges(const PolygonalMesh& mesh) {
	bool allEdges = true;
    for (unsigned int id = 0; id < mesh.NumCell1Ds; ++id) {
        double x0 = mesh.Cell0DsCoordinates(0, mesh.Cell1DsExtrema(0, id));  
        double y0 = mesh.Cell0DsCoordinates(1, mesh.Cell1DsExtrema(0, id));  
        double x1 = mesh.Cell0DsCoordinates(0, mesh.Cell1DsExtrema(1, id));  
        double y1 = mesh.Cell0DsCoordinates(1, mesh.Cell1DsExtrema(1, id));  

        double length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));

        // Verifica che la lunghezza sia maggiore di una tolleranza 
        if (length <= 1e-10)
		{
			cerr << "Edge with ID " << mesh.Cell1DsId[id] << " has zero length!" << endl;
			allEdges = false;
		}
	}
	return allEdges;
}


bool TestArea(const PolygonalMesh& mesh)
{
	bool allPol = true;
    for (size_t i = 0; i < mesh.NumCell2Ds; ++i)
    {
        const vector<unsigned int>& vertexIds = mesh.Cell2DsVertices[i];
        size_t numVertices = vertexIds.size();

        if (numVertices < 3)
        {
            cerr << "Polygon with ID " << mesh.Cell2DsId[i] << " has less than 3 vertices!" << endl;
            allPol = false;
            continue;
        }

        double area = 0.0;

        for (size_t j = 0; j < numVertices; ++j)
        {
            unsigned int id1 = vertexIds[j];
            unsigned int id2 = vertexIds[(j + 1) % numVertices]; // Per chiudere il poligono

            double x1 = mesh.Cell0DsCoordinates(0, id1);
            double y1 = mesh.Cell0DsCoordinates(1, id1);

            double x2 = mesh.Cell0DsCoordinates(0, id2);
            double y2 = mesh.Cell0DsCoordinates(1, id2);

            area += (x1 * y2) - (x2 * y1);
        }

        area = 0.5 * abs(area);

        if (area <= 0.0)
        {
            cerr << "Polygon with ID " << mesh.Cell2DsId[i] << " has zero or negative area!" << endl;
            allPol = false;
        }
    }

    return allPol;
}

bool TestMarker(PolygonalMesh& mesh)
{
	bool allMark = true;
	for(const auto& marker_coppie : mesh.MarkerCell0Ds)
		for(size_t id : marker_coppie.second)
			if (id >= mesh.Cell0DsId.size() || mesh.Cell0DsId[id] != id)
			{	
				allMark = false;
				cout << "cell0" << endl;
			}
			
	
			
	for(const auto& marker_coppie : mesh.MarkerCell1Ds)
		for(size_t id : marker_coppie.second)
			if (id >= mesh.Cell1DsId.size() || mesh.Cell1DsId[id] != id)
				{	
				allMark = false;
				cout << "cell1" << endl;
				}
			
	for(const auto& marker_coppie : mesh.MarkerCell2Ds)
		for(size_t id : marker_coppie.second)
			if (id >= mesh.Cell2DsId.size() || mesh.Cell2DsId[id] != id)
				{	
				allMark = false;
				cout << "cell2" << endl;
				}
	return allMark;
}
}



