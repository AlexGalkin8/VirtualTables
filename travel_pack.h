#pragma once

#include "identity_document.h"
#include "passport.h"
#include "driving_licence.h"

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class TravelPack
{
public:
    TravelPack()
        : parent()
        , identity_doc1_((IdentityDocument*) new Passport())
        , identity_doc2_((IdentityDocument*) new DrivingLicence())
    {
        TravelPack::SetVTable(this);
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
        : parent(other.parent)
        , identity_doc1_((IdentityDocument*) new Passport(*reinterpret_cast<Passport*>(other.identity_doc1_)))
        , identity_doc2_((IdentityDocument*) new DrivingLicence(*reinterpret_cast<DrivingLicence*>(other.identity_doc2_)))
        , additional_pass_(other.additional_pass_)
        , additional_dr_licence_(other.additional_dr_licence_)
    {
        TravelPack::SetVTable(const_cast<TravelPack*>(&other));
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }

    ~TravelPack()
    {
        identity_doc1_->Delete();
        identity_doc2_->Delete();
        std::cout << "TravelPack::Dtor()"sv << std::endl;
        //IdentityDocument::SetVTable((IdentityDocument*)this);
    }



    /*********************   Виртуальные методы класса   **************************/

    static void PrintID(const TravelPack* obj)
    {
        obj->identity_doc1_->PrintID();
        obj->identity_doc2_->PrintID();
        obj->additional_pass_.PrintID();
        obj->additional_dr_licence_.PrintID();
    }

    void PrintID() const
    {
        PrintID(this);
    }

    static void Delete(TravelPack* obj)
    {
        delete obj;
    }

    void Delete()
    {
        Delete(this);
    }


    /*********************   Унаследованные методы родителя   **************************/

    int GetID() const
    {
        return parent.GetID();
    }

    static void PrintUniqueIDCount()
    {
        IdentityDocument::PrintUniqueIDCount();
    }


    operator IdentityDocument() { return { parent }; }

    /*********************   Функции, для работы с вирт. таблицей   **************************/

    static void SetVTable(TravelPack* obj)
    {
        *(TravelPack::VTable**)obj = &TravelPack::vtable;
    }


private:
    struct VTable
    {
        using DeleteType = void (*)(TravelPack*);
        using PrintIDType = void (*)(const TravelPack*);

        DeleteType delete_func;
        PrintIDType print_id_func;
    };

private:
    IdentityDocument parent;
    static VTable vtable;

    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;

    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;
};

TravelPack::VTable TravelPack::vtable = { TravelPack::Delete, TravelPack::PrintID };
