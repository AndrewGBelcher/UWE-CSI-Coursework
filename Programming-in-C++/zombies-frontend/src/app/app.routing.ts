import { ModuleWithProviders } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { AboutComponent } from './about/about.component';
import { HomeComponent } from './home/home.component';
import { OutputPanelComponent } from './output-panel/output-panel.component';
import { ConnectComponent } from './connect/connect.component';

const appRoutes: Routes = [
  { path: 'about', component: AboutComponent },
        {
          path: 'connect',
          component: ConnectComponent,
        },
        {
          path: 'game',
          component: OutputPanelComponent
        },
  {
    path: '',
    redirectTo: '/connect',
    pathMatch: 'full'
  }

//  { path: '**', component: PageNotFoundComponent }
];

export const appRoutingProviders: any[] = [

];

export const routing: ModuleWithProviders = RouterModule.forRoot(appRoutes);
