#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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
		marker = stoi(token);

		getline(converter, token, ';');
		x = stod(token);

		getline(converter, token, ';');
		y = stod(token);

		
		mesh.Cell0DsCoordinates(0, id) = x;
		mesh.Cell0DsCoordinates(1, id) = y;
		mesh.Cell0DsId.push_back(id);
        
		Vector2d coord;

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
        marker = stoi(token);

        getline(converter, token, ';');
        origin = stoi(token);

        getline(converter, token, ';');
        end = stoi(token);

        mesh.Cell1DsId.push_back(id); 
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

}
