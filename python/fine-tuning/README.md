# Ajustement fin

Ce dossier se concentre sur l'ajustement fin du modèle Qwen3, qui est très performant et optimisé. L'objectif est de créer des notebooks simples pour ajuster finement les modèles avec plusieurs jeux de données.

L'idée est d'utiliser la version 4B de Qwen3 pour la communication avec le robot et de modifier le modèle 6M, qui est beaucoup plus petit, pour effectuer des actions plus précise.

## Aperçu

- **Modèle principal** : Qwen3-4B pour la communication avec l'agent ( robot )
- **Modèle secondaire** : Qwen3-6M pour l'exécution d'actions limitées
- **Approche** : Ajustement fin basé sur des notebooks simples avec divers jeux de données

## Structure

Ce dépôt contient des notebooks et des ressources pour :
- Préparation des jeux de données
- Flux de travail d'ajustement fin des modèles
- Évaluation des performances
- Exemples d'intégration